// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomUtilitiesManager.h"
#include "SymptomRelatedClasses/DestructibleFloor.h"
#include "SymptomsManager.generated.h"

#define MAX_SYMPTOMS 2

USTRUCT()
struct FSymptom
{
	GENERATED_BODY()

	// Actor affected by a symptom
	UPROPERTY()
	AActor* SymptomActor;

	// Duration of the symptom; decremented each tick in UpdateActiveSymptoms
	UPROPERTY()
	FTimespan Duration;

	// Index of the symptom's "effect" function in SymptomFunctions
	UPROPERTY()
	int32 SymptomEffectIndex;

	// Symptom constructor
	FSymptom(AActor* TaggedActor, FTimespan SymptomDuration, int32 EffectIndex)
		: SymptomActor(TaggedActor), Duration(SymptomDuration), SymptomEffectIndex(EffectIndex) {}
		
	// Default symptom constructor (mainly used for testing)
	FSymptom() : SymptomActor(NULL), Duration(FTimespan(0, 0, 10)), SymptomEffectIndex(0) {}

	FORCEINLINE bool operator==(const FSymptom& rhs)
	{
		return (this->SymptomActor == rhs.SymptomActor) && (this->SymptomEffectIndex == rhs.SymptomEffectIndex);
	}
};

UCLASS()
class HARLOWS_WALLPAPER_API ASymptomsManager : public AActor
{
	GENERATED_BODY()

private:
	// Function pointer taking a single Actor argument
	typedef void(ASymptomsManager::*SingleActorFunc)(AActor*);

	// What effect/action each symptom causes
	// Called every tick
	const SingleActorFunc SymptomFunctions[MAX_SYMPTOMS] = { &ASymptomsManager::Flee,  &ASymptomsManager::FallingFloor };

	// Array of actors with active Symptoms
	UPROPERTY()
	TArray<FSymptom> SymptomActors;

	// Decrements each symptom's durations by the given DeltaTime (called in Tick)
	// Removes any expired symptoms
	UFUNCTION()
	void UpdateActiveSymptoms(float DeltaTime);

	/* Symptom functions */

	// Flee from player
	UFUNCTION()
	void Flee(AActor* SymptomActor);

	//Floor falls to ruin
	UFUNCTION()
	void FallingFloor(AActor* SymptomActor);

	// See the note for the `DebugSymptomTickFrequency` property
	// We keep `DebugSymptomCanTickThisFrame` as a class-scoped variable so other methods (namely the
	// symptom functions themselves) only log with the specified frequency as well.
	float DebugSymptomTickCounter = 0.0f;
	bool DebugSymptomCanTickThisFrame = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ASymptomsManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Changes how frequently this actor ticks (and thus how often symptoms are run)
	// If <= 0 we'll ignore this value and tick every frame
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SymptomManager")
		float TickInterval = 0.0f;

	/**
	 * Tags an actor with a Symptom
	 * 
	 * @return true if the symptom was successfully added
	 */
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	bool AddSymptomToActor(AActor* Actor, const FName SymptomTag);

	// Don't log symptom durations every tick or we'll spam the logs
	// Instead increment a counter each tick and log if it says it's been long enough since the last tick
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		float DebugSymptomTickFrequency = 1.0f;
};