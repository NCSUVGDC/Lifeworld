// Fill out your copyright notice in the Description page of Project Settings.

#include "Shapeshifting.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include <random>

// Sets default values
AShapeshifting::AShapeshifting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShapeshifting::BeginPlay()
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
				if (Symptom == FName("Shapeshifting"))
				{
					// Add the valid actor to the validActors global variable and remove its tag
					validActors.Add(symptomActor);
					UE_LOG(LogTemp, Warning, TEXT("Added %s"), *symptomActor->GetName());
					symptomActor->Tags.Remove(FName("Shapeshifting"));
				}
			}
		}
	}

}

// Called every frame
void AShapeshifting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Turns the static mesh of object into mesh, and the material interface into material
UStaticMeshComponent* AShapeshifting::Shapeshift(AActor *object, UStaticMesh *mesh, UMaterialInterface *material)
{
	// If the object is part of the valid shapeshifting actors
	if (object != NULL && validActors.Contains(object) && mesh != NULL && material != NULL)
	{
		// Get the object's static mesh component
		TArray<UStaticMeshComponent*> components;
		object->GetComponents<UStaticMeshComponent>(components);
		UStaticMeshComponent *meshComponent = components[0];

		// Store the object's original static mesh component for returning later
		UStaticMeshComponent *origMeshComponent = NewObject<UStaticMeshComponent>(this);
		origMeshComponent->SetStaticMesh(meshComponent->GetStaticMesh());
		origMeshComponent->SetMaterial(0, meshComponent->GetMaterial(0));

		// Duplicate actor ignore collisions
		if (UPrimitiveComponent* PrimitiveComponent = object->FindComponentByClass<UPrimitiveComponent>())
		{
			PrimitiveComponent->SetEnableGravity(false);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

			// Set the static mesh and material to the new mesh and material
			meshComponent->SetStaticMesh(mesh);
			meshComponent->SetMaterial(0, material);


			TArray<AActor*> OverlapActors;
			PrimitiveComponent->GetOverlappingActors(OverlapActors);
			for (int i = 0; i < OverlapActors.Num(); i++) {
				//UE_LOG(LogTemp, Warning, TEXT("Overlapping is %s"), *OverlapActors[i]->GetName());
				//UE_LOG(LogTemp, Warning, TEXT("Roll is %f"), abs(object->GetActorRotation().Roll));
				//UE_LOG(LogTemp, Warning, TEXT("Pitch is %f"), abs(object->GetActorRotation().Pitch));
				//UE_LOG(LogTemp, Warning, TEXT("Cup location is %f"), object->GetActorLocation().Z);
				//UE_LOG(LogTemp, Warning, TEXT("Other location is %f"), OverlapActors[i]->GetActorLocation().Z);
				if (OverlapActors[i]->GetActorLocation().Z > object->GetActorLocation().Z && (abs(object->GetActorRotation().Roll) < 90 || abs(object->GetActorRotation().Pitch) < 90)) {
					if (OverlapActors[i]->IsRootComponentMovable()) {
						meshComponent->SetStaticMesh(origMeshComponent->GetStaticMesh());
						meshComponent->SetMaterial(0, origMeshComponent->GetMaterial(0));
						UE_LOG(LogTemp, Warning, TEXT("Bad actor is %s"), *OverlapActors[i]->GetName());
					}
				}
				if (OverlapActors[i]->GetActorLocation().Z < object->GetActorLocation().Z && (abs(object->GetActorRotation().Roll) > 90 || abs(object->GetActorRotation().Pitch) > 90)) {
					if (OverlapActors[i]->IsRootComponentMovable()) {
						meshComponent->SetStaticMesh(origMeshComponent->GetStaticMesh());
						meshComponent->SetMaterial(0, origMeshComponent->GetMaterial(0));
						UE_LOG(LogTemp, Warning, TEXT("Bad actor is %s"), *OverlapActors[i]->GetName());
					}
				}
			}

			PrimitiveComponent->SetEnableGravity(true);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}

		

		return origMeshComponent;
	}
	return NULL;
}

// Returns a random actor from all valid actors in the scene
AActor* AShapeshifting::SelectRandomAll()
{
	if (validActors.Num() != 0) {
		std::random_device rd;			// Only used once to initialize (seed) engine
		std::mt19937 rng(rd());			// Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, validActors.Num() - 1); // Sets range of numbers
		auto random_integer = uni(rng); // Picks random number within range
		return validActors[random_integer];
	} else {
		return NULL;
	}
}

// Returns a random actor from all valid and currently unseen actors in the scene
AActor* AShapeshifting::SelectRandomUnseen()
{
	if (validActors.Num() == 0) {
		return NULL;
	}

	// Array of valid unseen actors
	TArray<AActor*> validUnseenActors;

	// Creates an array of unseen valid actors in the scene based on their dot products
	AActor *loopActor;
	for (int i = 0; i < validActors.Num(); i++) {
		loopActor = validActors[i];										// Sets loopActor to next one in array
		FVector loopActorLocation = loopActor->GetActorLocation();		// Gets location of the loopActor
		AActor *cameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as actor
		FVector playerLocation = cameraAct->GetActorLocation();			// Gets player camera location
		FVector distanceVector = loopActorLocation - playerLocation;	// Gets distance vector between loopActor and player camera
		distanceVector = distanceVector / distanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
		FVector cameraVector = lifeworldCamera->GetForwardVector();		// Gets forward vector of the camera
		cameraVector = cameraVector / cameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
		float dotProd = FVector::DotProduct(distanceVector, cameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

		/// States what the Dot Product of each loopActor is
		UE_LOG(LogTemp, Log, TEXT("%s Dot Product: %f"), *loopActor->GetName(), dotProd);

		// If negative dot product, add to valid unseen actor array
		if (dotProd < 0.0) {
			validUnseenActors.Add(loopActor);
		}
	}

	// If there are valid unseen actors, picks a random one. Else, return null
	if (validUnseenActors.Num() != 0) {
		std::random_device rd;			// Only used once to initialize (seed) engine
		std::mt19937 rng(rd());			// Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, validUnseenActors.Num() - 1); // Sets range of numbers
		auto random_integer = uni(rng); // Picks random number within range
		return validUnseenActors[random_integer];
	} else {
		return NULL;
	}
}

// Validates that an actor is unseen at the moment, returning true if unseen
bool AShapeshifting::ValidateUnseen(AActor *object)
{
	if (object == NULL) {
		return NULL;
	}

	FVector loopActorLocation = object->GetActorLocation();		// Gets location of the loopActor
	AActor *cameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as actor
	FVector playerLocation = cameraAct->GetActorLocation();			// Gets player camera location
	FVector distanceVector = loopActorLocation - playerLocation;	// Gets distance vector between loopActor and player camera
	distanceVector = distanceVector / distanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
	FVector cameraVector = lifeworldCamera->GetForwardVector();		// Gets forward vector of the camera
	cameraVector = cameraVector / cameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
	float dotProd = FVector::DotProduct(distanceVector, cameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

	if (dotProd < 0.0) {
		return true;
	} else {
		return false;
	}
}

