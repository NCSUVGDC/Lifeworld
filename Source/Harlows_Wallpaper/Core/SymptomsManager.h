// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomUtilitiesManager.h"
#include "SymptomsManager.generated.h"

#define MAX_SYMPTOMS 1

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
	const SingleActorFunc SymptomFunctions[MAX_SYMPTOMS] = { &ASymptomsManager::Flee };

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ASymptomsManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tags an actor with a Symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	void AddSymptomToActor(AActor* Actor, const FName SymptomTag);
};