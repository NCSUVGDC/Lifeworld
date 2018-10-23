// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomsManager.generated.h"

// Not sure if this fits in with gameplay tags.
// Need to explore this further
UENUM(BlueprintType)
enum ESymptomTypes
{
	FLEE	UMETA(DisplayName = "Flee")
};

USTRUCT()
struct FSymptom
{
	GENERATED_BODY()

	UPROPERTY()
	// actor affected by a symptom
	AActor* TaggedActor;
	UPROPERTY()
	// duration of the symptom
	FTimespan Duration;
};

UCLASS()
class HARLOWS_WALLPAPER_API ASymptomsManager : public AActor
{
	GENERATED_BODY()

private:
	// Array of actors with active Symptoms
	UPROPERTY()
	TArray<FSymptom> SymptomActors;

public:	
	// Sets default values for this actor's properties
	ASymptomsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Removes specified symptom from the tagged actor.
	// Mainly used when adding newly tagged actors to symptom array
	void RemoveSymptomFromActor(AActor* TaggedActor); // may change

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Adds an Actor with an active symptom
	UFUNCTION(BlueprintCallable, Category="SymptomManager")
	void AddActorWithActiveSymptom(const AActor* SymptomActor);

	// Removes actors with an expired symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	void RemoveExpiredSymptoms();

	// Tags an actor with a Symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	void TagActorWithSymptom(AActor* Actor, ESymptomTypes SymptomType); // may change
};
