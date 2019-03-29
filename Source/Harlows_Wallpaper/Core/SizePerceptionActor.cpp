// Fill out your copyright notice in the Description page of Project Settings.

#include "SizePerceptionActor.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"


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

	// Gets the number of seconds since game started as an int
	float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	int32 realtimeSecondsInt = (int32)realtimeSeconds;
	
	// Checks if game time is during one of the trigger times for SizePerception
	switch (realtimeSecondsInt){
	case 5:
		if (proceed) {
			UE_LOG(LogTemp, Warning, TEXT("ACTIVATED ONLY ONCE"));
			Select();

			//INSERT CODE HERE FOR DUMMY TRIGGER

			proceed = false;	// Makes it so Select() cannot be ran until no longer a trigger time
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

	// Checks that LifeWorldPlayer is the correct pawn [CAN BE DELETED]
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
		FVector PlayerLocation = LifeWorldPlayer->GetActorLocation();
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
	AActor* DefaultActor;

	// Cycles through the array of dot products to determine which one fits criteria
	for (int i = 0; i < DotProducts.Num(); i++) {
		if (DotProducts[i] < 0.0) {
			LoopActor = SizePerceptionActors[i];
			FVector LoopActorLocation = LoopActor->GetActorLocation();
			FVector PlayerLocation = LifeWorldPlayer->GetActorLocation();
			FVector DistanceVector = LoopActorLocation - PlayerLocation;
			UE_LOG(LogTemp, Log, TEXT("Distance to %s : %f"), *LoopActor->GetName(), DistanceVector.Size());
			if (DistanceVector.Size() > largestDistance) {
				largestDistance = DistanceVector.Size();
				DefaultActor = LoopActor;
				UE_LOG(LogTemp, Warning, TEXT("DefaultActor became: %s"), *DefaultActor->GetName());
			}
			if (DistanceVector.Size() >= 130) {
				FinalSelectionRandomizer.Add(LoopActor);
				UE_LOG(LogTemp, Warning, TEXT("Added to FinalSelectionRandomizer: %s"), *LoopActor->GetName());
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("FINAL SELECTEDACTOR IS: %s"), *SelectedActor->GetName());
	return SelectedActor;
}