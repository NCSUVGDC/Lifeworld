// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomsManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"

// Sets default values
ASymptomsManager::ASymptomsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASymptomsManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Handle actors in world with pre-assigned Symptoms
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// Add actor to symptom array
	for (AActor* SymptomActor : FoundActors)
	{
		TArray<FName> Tags = SymptomActor->Tags;
		// check for active symptoms
		if (Tags.Num() != 0)
		{
			for (FName Symptom : Tags)
			{
				AddSymptomToActor(SymptomActor, Symptom);
			}

			// Remove tags
			SymptomActor->Tags.Empty();
		}
	}
}

// Called every frame
void ASymptomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// run symptom effects
	for (int32 i = SymptomActors.Num() - 1; i >= 0; --i)
	{
		FSymptom *Symptom = &(SymptomActors[i]);
		(this->* (SymptomFunctions[Symptom->SymptomEffectIndex]))(Symptom->SymptomActor);

		// expire symptoms
		ExpireActiveSymptoms();
	}
}

void ASymptomsManager::AddSymptomToActor(AActor* Actor, const FName Symptom)
{
	// Find corresponding duration and effect for Symptom
	FSymptomDetail *Detail = USymptomUtilitiesManager::SymptomDetails.Find(Symptom);

	// add symptom and remove tags
	SymptomActors.Add(FSymptom(Actor, Detail->SymptomDuration, Detail->SymptomEffectIndex));
	UE_LOG(LogTemp, Warning, TEXT("Added %s to active symptoms"), *(Actor->GetName()));
}

void ASymptomsManager::ExpireActiveSymptoms()
{
	static FTimespan Decrement = FTimespan(0, 0, 1); // decrement a second each cycle
	for (int32 i = SymptomActors.Num() - 1; i >= 0; --i)
	{
		// decrement duration
		FSymptom* Symptom = &SymptomActors[i];
		Symptom->Duration -= Decrement;
		UE_LOG(LogTemp, Warning, TEXT("%f seconds remaining for active symptom in %s"), Symptom->Duration.GetTotalSeconds(), *(Symptom->SymptomActor->GetName()));

		// remove if expired
		if (Symptom->Duration.GetTotalSeconds() <= 0.0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s with Symptom ID %d expired"), *(Symptom->SymptomActor->GetName()), Symptom->SymptomEffectIndex);
			FSymptom Copy = *Symptom; // need a copy, otherwise, we create a dangling ptr
			SymptomActors.Remove(Copy); // remove expired symptom
			UE_LOG(LogTemp, Warning, TEXT("symptom %d for actor %s successfully removed"), Symptom->SymptomEffectIndex, *(Symptom->SymptomActor->GetName()));
		}
	}
}

/* Symtpom Functions */
void ASymptomsManager::Flee(AActor * SymptomActor)
{
	if (GEngine)
	{
		// just print that we're executing successfully, for now
		FString DebugMsg = FString::Printf(TEXT("Running Flee Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}
	UE_LOG(LogTemp, Warning, TEXT("Running Flee Symptom on Actor %s"), *SymptomActor->GetName());
	// TODO: actor flees from player's sight
}

