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

	UE_LOG(LogTemp, Log, TEXT("%d symptoms available"), USymptomUtilitiesManager::SymptomDetails.Num());
	
	// Handle actors in world with pre-assigned Symptoms via the Tag system
	// This is done once here on BeginPlay; any future symptoms should be added via AddSymptomToActor
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

				bool SuccessfullyAddedSymptom = AddSymptomToActor(SymptomActor, Symptom);
				if (SuccessfullyAddedSymptom)
				{
					// No point in keeping the tag now that the symptom has been applied
					SymptomActor->Tags.RemoveAt(SymptomIdx);
				}
			}
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

		// The inner most parentheses with `SymptomFunctions[Symptom->SymptomEffectIndex]` gets the
		// symptom function pointer; the `this->*` dereferences that function pointer and calls it;
		// The finally parentheses with `Symptom->SymptomActor` are the function args
		(this->* (SymptomFunctions[Symptom->SymptomEffectIndex]))(Symptom->SymptomActor);
	}

	// Tick symptom durations
	UpdateActiveSymptoms(DeltaTime);
}

bool ASymptomsManager::AddSymptomToActor(AActor* Actor, const FName Symptom)
{
	// Find corresponding duration and effect for Symptom
	FSymptomDetails *Details = USymptomUtilitiesManager::SymptomDetails.Find(Symptom);

	// Dereferencing null pointers causes a hard crash - best to catch it here and keep an eye on the log
	if (Details == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find symptom details for symptom '%s'! Could not add to actor '%s'!"), 
			*Symptom.ToString(), *Actor->GetName());
		return false;
	}

	// Add symptom and remove tags
	SymptomActors.Add(FSymptom(Actor, Details->SymptomDuration, Details->SymptomEffectIndex));
	UE_LOG(LogTemp, Warning, TEXT("Added %s to active symptom actors"), *(Actor->GetName()));
	return true;
}

void ASymptomsManager::UpdateActiveSymptoms(float DeltaTime)
{
	// We'll be decrementing by the same amount for each symptom
	static FTimespan Decrement = FTimespan::FromSeconds(DeltaTime);

	for (int32 i = SymptomActors.Num() - 1; i >= 0; --i)
	{
		// Decrement duration
		FSymptom* Symptom = &SymptomActors[i];
		Symptom->Duration -= Decrement;
		UE_LOG(LogTemp, Log, TEXT("%f seconds remaining for active symptom in %s"), Symptom->Duration.GetTotalSeconds(), *(Symptom->SymptomActor->GetName()));

		// Remove if expired
		if (Symptom->Duration.GetTotalSeconds() <= 0.0)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s with Symptom ID %d expired"), *(Symptom->SymptomActor->GetName()), Symptom->SymptomEffectIndex);
			FSymptom Copy = *Symptom; // need a copy, otherwise, we create a dangling ptr
			SymptomActors.Remove(Copy); // remove expired symptom
			UE_LOG(LogTemp, Warning, TEXT("Symptom %d for actor %s successfully removed"), Symptom->SymptomEffectIndex, *(Symptom->SymptomActor->GetName()));
		}
	}
}

/* Symptom Functions */
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

