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
					///UE_LOG(LogTemp, Warning, TEXT("Added %s"), *symptomActor->GetName());
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
		if (components.Num() == 0) {
			return NULL;
		}
		UStaticMeshComponent *meshComponent = components[0];

		// Store the object's original static mesh component for returning later
		UStaticMeshComponent *origMeshComponent = NewObject<UStaticMeshComponent>(this);
		origMeshComponent->SetStaticMesh(meshComponent->GetStaticMesh());
		origMeshComponent->SetMaterial(0, meshComponent->GetMaterial(0));

		// Collision check block
		if (UPrimitiveComponent* PrimitiveComponent = object->FindComponentByClass<UPrimitiveComponent>())
		{
			// Temporarily disable gravity and collisions
			PrimitiveComponent->SetEnableGravity(false);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

			// Set the static mesh and material to the new mesh and material
			meshComponent->SetStaticMesh(mesh);
			meshComponent->SetMaterial(0, material);

			// Get the the actors that touch it and iterate through them
			TArray<AActor*> OverlapActors;
			PrimitiveComponent->GetOverlappingActors(OverlapActors);
			for (int i = 0; i < OverlapActors.Num(); i++) {
				///UE_LOG(LogTemp, Warning, TEXT("Overlapping is %s"), *OverlapActors[i]->GetName());
				///UE_LOG(LogTemp, Warning, TEXT("Roll is %f"), abs(object->GetActorRotation().Roll));
				///UE_LOG(LogTemp, Warning, TEXT("Pitch is %f"), abs(object->GetActorRotation().Pitch));
				///UE_LOG(LogTemp, Warning, TEXT("Cup location is %f"), object->GetActorLocation().Z);
				///UE_LOG(LogTemp, Warning, TEXT("Other location is %f"), OverlapActors[i]->GetActorLocation().Z);
				
				// If an actor is above the object and the object is right-side up, cancel shapeshift
				if (OverlapActors[i]->GetActorLocation().Z > object->GetActorLocation().Z && (abs(object->GetActorRotation().Roll) < 90 || abs(object->GetActorRotation().Pitch) < 90)) {
					if (OverlapActors[i]->IsRootComponentMovable()) {
						meshComponent->SetStaticMesh(origMeshComponent->GetStaticMesh());
						meshComponent->SetMaterial(0, origMeshComponent->GetMaterial(0));
						PrimitiveComponent->SetEnableGravity(true);
						PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
						return NULL;
						///UE_LOG(LogTemp, Warning, TEXT("Bad actor is %s"), *OverlapActors[i]->GetName());
					}
				}

				// If an actor is below the object and the object is upside-down, cancel shapeshift
				if (OverlapActors[i]->GetActorLocation().Z < object->GetActorLocation().Z && (abs(object->GetActorRotation().Roll) > 90 || abs(object->GetActorRotation().Pitch) > 90)) {
					if (OverlapActors[i]->IsRootComponentMovable()) {
						meshComponent->SetStaticMesh(origMeshComponent->GetStaticMesh());
						meshComponent->SetMaterial(0, origMeshComponent->GetMaterial(0));
						PrimitiveComponent->SetEnableGravity(true);
						PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
						return NULL;
						///UE_LOG(LogTemp, Warning, TEXT("Bad actor is %s"), *OverlapActors[i]->GetName());
					}
				}
			}

			// Enable gravity and collisions
			PrimitiveComponent->SetEnableGravity(true);
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		}

		// Return original mesh component
		return origMeshComponent;
	}

	// Return null if shapeshift did not occur
	return NULL;
}

TArray<AActor*> AShapeshifting::getShapeshiftActors() const
{
	return validActors;
}
