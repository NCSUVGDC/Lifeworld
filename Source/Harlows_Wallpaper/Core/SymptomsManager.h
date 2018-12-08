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

	UPROPERTY()
	// actor affected by a symptom
	AActor* SymptomActor;
	UPROPERTY()
	// duration of the symptom
	FTimespan Duration;
	UPROPERTY()
	// symptom effect index
	int32 SymptomEffectIndex;

	// symptom constructor
	FSymptom(AActor* TaggedActor, FTimespan SymptomDuration, int32 EffectIndex)
		: SymptomActor(TaggedActor), Duration(SymptomDuration), SymptomEffectIndex(EffectIndex) {}
	// default symptom constructor (mainly used for testing)
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
	// function pointer taking a single Actor argument
	typedef void(ASymptomsManager::*SingleActorFunc)(AActor*);

	// symptom functions array
	const SingleActorFunc SymptomFunctions[MAX_SYMPTOMS] = { &ASymptomsManager::Flee };

	// Array of actors with active Symptoms
	UPROPERTY()
	TArray<FSymptom> SymptomActors;

	UFUNCTION()
	void ExpireActiveSymptoms();

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