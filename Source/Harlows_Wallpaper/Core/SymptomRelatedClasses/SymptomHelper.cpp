// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomHelper.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include <random>

// Sets default values
ASymptomHelper::ASymptomHelper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASymptomHelper::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASymptomHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* ASymptomHelper::SelectRandomAll(TArray<AActor*> actors)
{
	if (actors.Num() != 0) {
		std::random_device rd;			// Only used once to initialize (seed) engine
		std::mt19937 rng(rd());			// Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, actors.Num() - 1); // Sets range of numbers
		auto random_integer = uni(rng); // Picks random number within range
		return actors[random_integer];
	}
	else {
		return NULL;
	}
}

AActor* ASymptomHelper::SelectRandomUnseen(TArray<AActor*> actors)
{
	if (actors.Num() == 0) {
		return NULL;
	}

	// Array of valid unseen actors
	TArray<AActor*> validUnseenActors;

	// Sets lifeworldPlayer to HarlowPawn
	APawn *lifeworldPlayer = UGameplayStatics::GetPlayerPawn(actors[0]->GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	USceneComponent *lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	// Creates an array of unseen valid actors in the scene based on their dot products
	AActor *loopActor;
	for (int i = 0; i < actors.Num(); i++) {
		loopActor = actors[i];											// Sets loopActor to next one in array
		FVector loopActorLocation = loopActor->GetActorLocation();		// Gets location of the loopActor
		AActor *cameraAct = GEngine->GetFirstLocalPlayerController(actors[i]->GetWorld())->PlayerCameraManager; // Gets player camera as actor
		FVector playerLocation = cameraAct->GetActorLocation();			// Gets player camera location
		FVector distanceVector = loopActorLocation - playerLocation;	// Gets distance vector between loopActor and player camera
		distanceVector = distanceVector / distanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
		FVector cameraVector = lifeworldCamera->GetForwardVector();		// Gets forward vector of the camera
		cameraVector = cameraVector / cameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
		float dotProd = FVector::DotProduct(distanceVector, cameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

		/// States what the Dot Product of each loopActor is
		///UE_LOG(LogTemp, Log, TEXT("%s Dot Product: %f"), *loopActor->GetName(), dotProd);

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
	}
	else {
		return NULL;
	}
}

bool ASymptomHelper::ValidateUnseen(AActor *object)
{
	if (object == NULL) {
		return NULL;
	}

	// Sets lifeworldPlayer to HarlowPawn
	APawn *lifeworldPlayer = UGameplayStatics::GetPlayerPawn(object->GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	USceneComponent *lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	FVector loopActorLocation = object->GetActorLocation();			// Gets location of the loopActor
	AActor *cameraAct = GEngine->GetFirstLocalPlayerController(object->GetWorld())->PlayerCameraManager; // Gets player camera as actor
	FVector playerLocation = cameraAct->GetActorLocation();			// Gets player camera location
	FVector distanceVector = loopActorLocation - playerLocation;	// Gets distance vector between loopActor and player camera
	distanceVector = distanceVector / distanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
	FVector cameraVector = lifeworldCamera->GetForwardVector();		// Gets forward vector of the camera
	cameraVector = cameraVector / cameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
	float dotProd = FVector::DotProduct(distanceVector, cameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

	if (dotProd < 0.0) {
		return true;
	}
	else {
		return false;
	}
}

bool ASymptomHelper::ValidateUnseenLocation(FVector location, AActor *object)
{
	// Sets lifeworldPlayer to HarlowPawn
	APawn *lifeworldPlayer = UGameplayStatics::GetPlayerPawn(object->GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	USceneComponent *lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	AActor *cameraAct = GEngine->GetFirstLocalPlayerController(object->GetWorld())->PlayerCameraManager; // Gets player camera as actor
	FVector playerLocation = cameraAct->GetActorLocation();			// Gets player camera location
	FVector distanceVector = location - playerLocation;				// Gets distance vector between location and player camera
	distanceVector = distanceVector / distanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
	FVector cameraVector = lifeworldCamera->GetForwardVector();		// Gets forward vector of the camera
	cameraVector = cameraVector / cameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
	float dotProd = FVector::DotProduct(distanceVector, cameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

	if (dotProd < 0.0) {
		return true;
	}
	else {
		return false;
	}
}

AActor* ASymptomHelper::CloneActor(AActor *object)
{
	FActorSpawnParameters params;
	params.Template = object;

	//Check if the actor is valid
	if (object)
	{
		UWorld* const World = object->GetWorld();
		if (World)
		{
			AActor *SpawnedActor = World->SpawnActor<AActor>(object->GetClass(), params);
			UE_LOG(LogTemp, Warning, TEXT("Created clone: %s"), *SpawnedActor->GetName());
			return SpawnedActor;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("World - False"));
			return NULL;
		}
	}
	//If the actor is not valid, return NULL
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Actor invalid"));
		return NULL;
	}
	
	
}
