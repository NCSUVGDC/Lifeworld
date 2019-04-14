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

	// Set a custom tick rate
	if (TickInterval > 0)
		SetActorTickInterval(TickInterval);

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

	// Tick symptom durations
	UpdateActiveSymptoms(DeltaTime);

	// run symptom effects
	for (int32 i = SymptomActors.Num() - 1; i >= 0; --i)
	{
		FSymptom *Symptom = &(SymptomActors[i]);

		// The inner most parentheses with `SymptomFunctions[Symptom->SymptomEffectIndex]` gets the
		// symptom function pointer; the `this->*` dereferences that function pointer and calls it;
		// The finally parentheses with `Symptom->SymptomActor` are the function args
		(this->* (SymptomFunctions[Symptom->SymptomEffectIndex]))(Symptom->SymptomActor);
	}
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
	// Check if it's safe to log; set CanTickThisFrame to false in case it was true last frame
	DebugSymptomCanTickThisFrame = false;
	DebugSymptomTickCounter += DeltaTime;
	if (DebugSymptomTickCounter >= DebugSymptomTickFrequency)
	{
		DebugSymptomCanTickThisFrame = true;
		DebugSymptomTickCounter -= DebugSymptomTickFrequency;
	}

	// We'll be decrementing by the same amount for each symptom
	FTimespan Decrement = FTimespan::FromSeconds(DeltaTime);

	// Log the decrement time if there are any symptomsto decrement
	if (SymptomActors.Num() > 0)
		UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Log, TEXT("Decrementing all symptoms by %s"), *Decrement.ToString());

	for (int32 SymptomIdx = SymptomActors.Num() - 1; SymptomIdx >= 0; SymptomIdx--)
	{
		// Decrement duration
		FSymptom* Symptom = &SymptomActors[SymptomIdx];
		Symptom->Duration -= Decrement;
		UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Log, TEXT("%f seconds remaining for active symptom in %s"), 
			Symptom->Duration.GetTotalSeconds(), *(Symptom->SymptomActor->GetName()));

		// Remove if expired
		if (Symptom->Duration.GetTotalSeconds() <= 0.0)
		{
			// Log about expirations regardless of the tick frequency; that's pertinent info.
			UE_LOG(LogTemp, Warning, TEXT("%s with Symptom ID %d expired"), *(Symptom->SymptomActor->GetName()), Symptom->SymptomEffectIndex);
			SymptomActors.RemoveAt(SymptomIdx); // remove expired symptom
		}
	}
}

/* Symptom Functions */
void ASymptomsManager::ImposeSizePerception(AActor * SymptomActor)
{
	if (DebugSymptomCanTickThisFrame && GEngine)
	{
		// just print that we're executing successfully, for now
		FString DebugMsg = FString::Printf(TEXT("Running SizePerception Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}
	
	UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Warning, TEXT("Running SizePerception Symptom on Actor %s"), *SymptomActor->GetName());
	// TODO: actor's size changes
}

void ASymptomsManager::ImposeFallingFloor(AActor * SymptomActor)
{
	ADestructibleFloor* current = (ADestructibleFloor*)SymptomActor;
	// Value is seconds before the start of recovery
	// Note* Timers are in seconds from start of symptom, floor falls between 4-10 seconds in
	current->StartSymptom(14);
}
void ASymptomsManager::ImposeVoices(AActor * SymptomActor) {}

void ASymptomsManager::ImposeDoubleTake(AActor * SymptomActor) {}

void ASymptomsManager::ImposeWarpingWalls(AActor * SymptomActor) {}

void ASymptomsManager::ImposePhantom(AActor * SymptomActor) {}

void ASymptomsManager::ImposeBackIsTurned(AActor * SymptomActor) {}

void ASymptomsManager::ImposeLensFlaring(AActor * SymptomActor) {}

void ASymptomsManager::ImposeBreatheIn(AActor * SymptomActor) {}
