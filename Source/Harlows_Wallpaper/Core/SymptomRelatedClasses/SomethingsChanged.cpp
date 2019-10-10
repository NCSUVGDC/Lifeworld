// Fill out your copyright notice in the Description page of Project Settings.

#include "SomethingsChanged.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASomethingsChanged::ASomethingsChanged()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASomethingsChanged::BeginPlay()
{
	Super::BeginPlay();

	// Sets lifeworldPlayer to HarlowPawn
	lifeworldPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	// Create an array of all actors in the scene
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);

	// For each actor in the scene...
	for (AActor* symptomActor : foundActors)
	{
		// Check for active symptoms...
		if (symptomActor->Tags.Num() != 0)
		{
			// Loop backwards over the list so we can safely remove tags as we iterate
			for (int symptomIdx = symptomActor->Tags.Num() - 1; symptomIdx >= 0; symptomIdx--)
			{
				// Gets the current tag in the list
				FName& Symptom = symptomActor->Tags[symptomIdx];

				// Check if the tag says it can be affected by Shapeshifting
				if (Symptom == FName("SomethingsChanged"))
				{
					// Add the valid actor to the validActors global variable and remove its tag
					validActors.Add(symptomActor);
					///UE_LOG(LogTemp, Warning, TEXT("Added %s"), *symptomActor->GetName());
					symptomActor->Tags.Remove(FName("SomethingsChanged"));
				}
			}
		}
	}

}

// Called every frame
void ASomethingsChanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UMaterialInterface * ASomethingsChanged::MaterialChanged(AActor * object, UMaterialInterface * material)
{
	// If the object is part of the valid actors
	if (object != NULL && validActors.Contains(object) && material != NULL)
	{
		// Get the object's static mesh component
		TArray<UStaticMeshComponent*> components;
		object->GetComponents<UStaticMeshComponent>(components);
		if (components.Num() == 0) {
			return NULL;
		}
		UStaticMeshComponent *meshComponent = components[0];

		// Store the object's original static mesh component for returning later
		UStaticMeshComponent *origMeshComponent = NewObject<UStaticMeshComponent>(this);
		origMeshComponent->SetStaticMesh(meshComponent->GetStaticMesh());
		origMeshComponent->SetMaterial(0, meshComponent->GetMaterial(0));

		// Change material of object
		meshComponent->SetMaterial(0, material);

		// Returns the original mesh component
		return origMeshComponent->GetMaterial(0);
	}

	// Return null if change did not occur
	return NULL;
}

FVector ASomethingsChanged::LocationChanged(AActor * object, FVector location)
{
	// If the object is part of the valid actors
	if (object != NULL && validActors.Contains(object) && location != FVector(0))
	{
		// Store the object's original location for returning later
		FVector origLocation = object->GetActorLocation();

		// Initialize the actor that might be on top of object and the difference in z values
		AActor* carriedActor = NULL;
		FVector carriedLocation = FVector(0);
		float diffX = 0;
		float diffY = 0;
		float diffZ = 0;

		// Object sitting on/under other actor check
		if (UPrimitiveComponent* PrimitiveComponent = object->FindComponentByClass<UPrimitiveComponent>())
		{
			// Disable object gravity and collisions so it can check touching objects
			PrimitiveComponent->SetEnableGravity(false);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			object->SetActorScale3D(FVector(object->GetActorScale3D().X, object->GetActorScale3D().Y, object->GetActorScale3D().Z + 0.1));

			// Get the the actors that touch it and iterate through them
			TArray<AActor*> OverlapActors;
			PrimitiveComponent->GetOverlappingActors(OverlapActors);

			// Enable gravity and collisions because we have the touching objects
			PrimitiveComponent->SetEnableGravity(true);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			object->SetActorScale3D(FVector(object->GetActorScale3D().X, object->GetActorScale3D().Y, object->GetActorScale3D().Z - 0.1));

			// Iterate through the touching objects
			for (int i = 0; i < OverlapActors.Num(); i++) {
				// If an actor is below object...
				if (OverlapActors[i]->GetActorLocation().Z < object->GetActorLocation().Z && OverlapActors[i]->IsRootComponentMovable() && OverlapActors[i]->GetClass()->GetName() != "BP_HarlowPawn_C" && !OverlapActors[i]->Tags.Contains("Moved")) {
					UE_LOG(LogTemp, Error, TEXT("Recursive going into lower actor %s"), *OverlapActors[i]->GetName());
					diffX = object->GetActorLocation().X - OverlapActors[i]->GetActorLocation().X;
					diffY = object->GetActorLocation().Y - OverlapActors[i]->GetActorLocation().Y;
					diffZ = object->GetActorLocation().Z - OverlapActors[i]->GetActorLocation().Z;
					carriedLocation = location;
					carriedLocation.X = location.X - diffX;
					carriedLocation.Y = location.Y - diffY;
					carriedLocation.Z = location.Z - diffZ;
					//object->bGenerateOverlapEventsDuringLevelStreaming = false;
					// Recursive call the lower actor, which should move everything above it. Return zero vector if fail, origLocation if succeed
					if (LocationChanged(OverlapActors[i], carriedLocation) == FVector(0)) {
						return FVector(0);
					} else {
						//object->bGenerateOverlapEventsDuringLevelStreaming = true;
						return origLocation;
					}
				}
				// If an actor is above object...
				if (OverlapActors[i]->GetActorLocation().Z > object->GetActorLocation().Z && OverlapActors[i]->IsRootComponentMovable() && OverlapActors[i]->GetClass()->GetName() != "BP_HarlowPawn_C") {
					// Store for later
					carriedActor = OverlapActors[i];
					diffX = object->GetActorLocation().X - OverlapActors[i]->GetActorLocation().X;
					diffY = object->GetActorLocation().Y - OverlapActors[i]->GetActorLocation().Y;
					diffZ = object->GetActorLocation().Z - OverlapActors[i]->GetActorLocation().Z;
					UE_LOG(LogTemp, Warning, TEXT("Actor above object is %s"), *OverlapActors[i]->GetName());
				}
			}

			// Disable object gravity and collisions
			PrimitiveComponent->SetEnableGravity(false);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

			// Change location of object
			location.Z = location.Z - 0.1;
			object->SetActorLocation(location);

			// Initiate safe to change location flag
			int safe = 0;

			// Get the actors that would touch when location changes and iterate
			PrimitiveComponent = object->FindComponentByClass<UPrimitiveComponent>();
			PrimitiveComponent->GetOverlappingActors(OverlapActors);
			UE_LOG(LogTemp, Warning, TEXT("Number of overlapping actors is %f"), float(OverlapActors.Num()));
			for (int i = 0; i < OverlapActors.Num(); i++) {
				// If an actor is below object and safe is default, set safe to 1
				if (OverlapActors[i]->GetActorLocation().Z < object->GetActorLocation().Z && safe == 0 && OverlapActors[i]->GetClass()->GetName() != "BP_HarlowPawn_C") {
					safe = 1;
					UE_LOG(LogTemp, Warning, TEXT("Safe set to 1 because %s"), *OverlapActors[i]->GetName());
				}
				// If an actor is above/inside object, set safe to -1
				if (OverlapActors[i]->GetActorLocation().Z >= object->GetActorLocation().Z && OverlapActors[i]->GetClass()->GetName() != "BP_HarlowPawn_C" && OverlapActors[i] != carriedActor) {
					safe = -1;
					UE_LOG(LogTemp, Warning, TEXT("Safe set to -1 because %s"), *OverlapActors[i]->GetName());
				}
			}

			// If unsafe, revert location, enable gravity/collisions, and return zero vector
			if (safe <= 0) {
				UE_LOG(LogTemp, Warning, TEXT("Not changing, safe is %d"), safe);
				object->SetActorLocation(origLocation);
				PrimitiveComponent->SetEnableGravity(true);
				PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				return FVector(0);
			}

			// Add a temporary tag stating that it's in the moving process to prevent duplicate recursions
			object->Tags.Add("Moved");

			// If there is an actor on top, move the actor as well
			if (carriedActor != NULL) {
				carriedLocation = location;
				carriedLocation.X = location.X - diffX;
				carriedLocation.Y = location.Y - diffY;
				carriedLocation.Z = location.Z - diffZ;
				UE_LOG(LogTemp, Error, TEXT("Recursive going into upper actor %s"), *carriedActor->GetName());
				LocationChanged(carriedActor, carriedLocation);
			}

			// Enable gravity and collisions
			PrimitiveComponent->SetEnableGravity(true);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

			// Remove the Moved tag
			object->Tags.Remove("Moved");
		}

		// Returns the original mesh component
		UE_LOG(LogTemp, Log, TEXT("Finished SomethingsChanged"));
		return origLocation;
	}

	// Returns zero vector if failed
	UE_LOG(LogTemp, Warning, TEXT("Instant failure with object %s"), *object->GetName());
	return FVector(0);
}

TArray<AActor*> ASomethingsChanged::MaterialsSwap(AActor * object0, AActor * object1)
{
	// If the object is part of the valid actors
	if (object0 != NULL && object1 != NULL && validActors.Contains(object0) && validActors.Contains(object1))
	{
		// Get the object0's static mesh component
		TArray<UStaticMeshComponent*> components0;
		object0->GetComponents<UStaticMeshComponent>(components0);
		if (components0.Num() == 0) {
			return TArray<AActor*>();
		}
		UStaticMeshComponent *meshComponent0 = components0[0];

		// Get the object1's static mesh component
		TArray<UStaticMeshComponent*> components1;
		object1->GetComponents<UStaticMeshComponent>(components1);
		if (components1.Num() == 0) {
			return TArray<AActor*>();
		}
		UStaticMeshComponent *meshComponent1 = components1[0];

		// Store the object0's original static mesh component for swapping
		UStaticMeshComponent *origMeshComponent0 = NewObject<UStaticMeshComponent>(this);
		origMeshComponent0->SetMaterial(0, meshComponent0->GetMaterial(0));

		// Swap material of objects
		meshComponent0->SetMaterial(0, meshComponent1->GetMaterial(0));
		meshComponent1->SetMaterial(0, origMeshComponent0->GetMaterial(0));

		// Returns an array of both actors, in order
		TArray<AActor*> ret;
		ret.Add(object0);
		ret.Add(object1);
		return ret;
	}

	// Return empty array if change did not occur
	return TArray<AActor*>();
}

TArray<FVector> ASomethingsChanged::LocationsSwap(AActor * object0, AActor * object1)
{
	return TArray<FVector>();
}

