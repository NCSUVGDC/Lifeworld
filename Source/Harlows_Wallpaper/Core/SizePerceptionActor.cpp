// Fill out your copyright notice in the Description page of Project Settings.

#include "SizePerceptionActor.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include <random>


// Sets default values
ASizePerceptionActor::ASizePerceptionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASizePerceptionActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("SizePerceptionActor BeginPlay"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// Add actors to symptom array
	for (AActor* SymptomActor : FoundActors)
	{
		// Check for active symptoms
		if (SymptomActor->Tags.Num() != 0)
		{
			// Loop backwards over the list so we can safely remove tags as we iterate
			for (int SymptomIdx = SymptomActor->Tags.Num() - 1; SymptomIdx >= 0; SymptomIdx--)
			{
				FName& Symptom = SymptomActor->Tags[SymptomIdx];

				// Check if the tag says it can be scaled by SizePerception
				if (Symptom == FName("SizePerception"))
				{
					SizePerceptionActors.Add(SymptomActor);
					UE_LOG(LogTemp, Warning, TEXT("Added Actor: %s"), *SymptomActor->GetName());
					SymptomActor->Tags.Remove(FName("SizePerception"));
				}
			}
		}
	}

	// Sets proceed to a value that wouldn't cause problems
	proceed = false;

	// Initializes DotProducts to an array of length of SizePerceptionActors
	DotProducts.SetNum(SizePerceptionActors.Num());
}

// Called every frame
void ASizePerceptionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScaleOriginal();

	// Gets the number of seconds since game started as an int
	float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	int32 realtimeSecondsInt = (int32)realtimeSeconds;
	
	// Checks if game time is during one of the trigger times for SizePerception
	switch (realtimeSecondsInt){
	case 10:
		if (proceed) {
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATED ONLY ONCE"));
			AActor* SPActor = Select();

			if (SPActor != NULL) {
				scaleSize = 0.25;
				tempScale = 0.25;
				float distance = SADistance;
				UE_LOG(LogTemp, Warning, TEXT("Distance to Scaled Item: %f"), distance);

				float scaleUnit = 1 + (((scaleSize - 1) / 210) * (distance - 50));

				FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);
				//FVector NewScale = FVector((distance / 260.0) * scaleSize, (distance / 260.0) * scaleSize, (distance / 260.0) * scaleSize);
				SPActor->SetActorScale3D(NewScale);
				proceed = false;	// Makes it so Select() cannot be ran until no longer a trigger time
			}
		}
		break;
	default:
		proceed = true;			// Reset proceed whenever it's not a trigger time
		break;
	}
}

// Locates the actor used in SizePerception
AActor* ASizePerceptionActor::Select()
{
	UE_LOG(LogTemp, Warning, TEXT("SELECT() TURNED ON"));
	
	// Sets LifeWorldPlayer to HarlowPawn
	LifeWorldPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Checks that LifeWorldPlayer is the correct pawn
	if (LifeWorldPlayer->Tags[0] == FName("Player")) {
		UE_LOG(LogTemp, Warning, TEXT("Got the player pawn correctly"));
	}

	// Sets LifeWorldCamera to the camera component of LifeWorldPlayer
	LifeWorldCamera = Cast<USceneComponent>(LifeWorldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	// Returned actor that'll be manipulated
	AActor* SelectedActor = NULL;

	// Creates array of dot products based on the actors in the scene
	AActor* LoopActor;
	for (int i = 0; i < SizePerceptionActors.Num(); i++) {
		LoopActor = SizePerceptionActors[i];
		FVector LoopActorLocation = LoopActor->GetActorLocation();
		AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
		FVector PlayerLocation = CameraAct->GetActorLocation();
		FVector DistanceVector = LoopActorLocation - PlayerLocation;
		DistanceVector = DistanceVector / DistanceVector.Size();	// Normalizes the distance vector (magnitude of 1)
		FVector CameraVector = LifeWorldCamera->GetForwardVector();
		CameraVector = CameraVector / CameraVector.Size();			// Normalizes the camera vector (magnitude of 1)
		float dotProd = FVector::DotProduct(DistanceVector, CameraVector);

		// Allows testing string
		UE_LOG(LogTemp, Log, TEXT("%s Dot Product: %f"), *LoopActor->GetName(), dotProd);
		
		DotProducts[i] = dotProd;
	}

	// Initializes FinalSelectionRandomizer to an empty array
	FinalSelectionRandomizer.SetNum(0);

	// Initializes the largestDistance and DefaultActor variables
	float largestDistance = 0;
	AActor* DefaultActor = NULL;

	// Cycles through the array of dot products to determine which ones are both behind the player and far away enough to be significant
	for (int i = 0; i < DotProducts.Num(); i++) {
		if (DotProducts[i] < 0.0) {
			LoopActor = SizePerceptionActors[i];
			FVector LoopActorLocation = LoopActor->GetActorLocation();
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
			FVector PlayerLocation = CameraAct->GetActorLocation();
			FVector DistanceVector = LoopActorLocation - PlayerLocation;
			UE_LOG(LogTemp, Log, TEXT("Distance to %s : %f"), *LoopActor->GetName(), DistanceVector.Size());
			// Sets the default actor if none are far away enough
			if (DistanceVector.Size() > largestDistance) {
				largestDistance = DistanceVector.Size();
				DefaultActor = LoopActor;
				UE_LOG(LogTemp, Warning, TEXT("DefaultActor became: %s"), *DefaultActor->GetName());
			}
			// If distance is significant enough, add the actor to FinalSelectionRandomizer array
			if (DistanceVector.Size() >= 100) {
				FinalSelectionRandomizer.Add(LoopActor);
				UE_LOG(LogTemp, Warning, TEXT("Added to FinalSelectionRandomizer: %s"), *LoopActor->GetName());
			}
		}
	}

	// Randomly picks an actor from FinalSelectionRandomizer if applicable, picks DefaultActor if not
	if (FinalSelectionRandomizer.Num() > 0) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, FinalSelectionRandomizer.Num() - 1); // guaranteed unbiased
		auto random_integer = uni(rng);
		AActor* ChosenThing = FinalSelectionRandomizer[random_integer];
		UE_LOG(LogTemp, Warning, TEXT("Chosen thing is: %s"), *ChosenThing->GetName());
		SelectedActor = ChosenThing;
		FVector SelectedActorLocation = SelectedActor->GetActorLocation();
		AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
		FVector PlayerLocation = CameraAct->GetActorLocation();
		FVector SADistanceVect = SelectedActorLocation - PlayerLocation;
		SADistance = SADistanceVect.Size();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Going with the DefaultActor because nothing is far enough"));
		SelectedActor = DefaultActor;
		//FVector SelectedActorLocation = SelectedActor->GetActorLocation();
		//FVector PlayerLocation = LifeWorldPlayer->GetActorLocation();
		//FVector SADistanceVect = SelectedActorLocation - PlayerLocation;
		//SADistance = SADistanceVect.Size();
	}
	

	if (SelectedActor != NULL) {
		UE_LOG(LogTemp, Warning, TEXT("FINAL SELECTEDACTOR IS: %s"), *SelectedActor->GetName());
	}
	return SelectedActor;
}

// Locates the actor used in SizePerception
void ASizePerceptionActor::ScaleOriginal() {
	for (int i = 0; i < SizePerceptionActors.Num(); i++) {
		AActor* LoopActor = SizePerceptionActors[i];
		FVector LAScale = LoopActor->GetActorScale();
		if (LAScale.Size() > 1.7320508 && tempScale > 1) {
			UE_LOG(LogTemp, Warning, TEXT("TEMPSCALE IS: %f"), tempScale);
			FVector LoopActorLocation = LoopActor->GetActorLocation();
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
			FVector PlayerLocation = CameraAct->GetActorLocation();
			FVector DistanceVect = LoopActorLocation - PlayerLocation;
			float distance = DistanceVect.Size();
		
			float scaleUnit = 1 + (((scaleSize - 1) / 210) * (distance - 50));
		
			if (scaleUnit < tempScale) {
				FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);
				LoopActor->SetActorScale3D(NewScale);
				tempScale = scaleUnit;
			}
		}
		else if (LAScale.Size() < 1.7120508 && tempScale < 1) {
			UE_LOG(LogTemp, Warning, TEXT("TEMPSCALE IS: %f"), tempScale);
			FVector LoopActorLocation = LoopActor->GetActorLocation();
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
			FVector PlayerLocation = CameraAct->GetActorLocation();
			FVector DistanceVect = LoopActorLocation - PlayerLocation;
			float distance = DistanceVect.Size();

			float scaleUnit = 1 + (((scaleSize - 1) / 210) * (distance - 50));

			if (scaleUnit > tempScale) {
				FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);
				LoopActor->SetActorScale3D(NewScale);
				tempScale = scaleUnit;
			}
		}
		else {
			LoopActor->SetActorScale3D(FVector(1, 1, 1));
		}
	}
}
