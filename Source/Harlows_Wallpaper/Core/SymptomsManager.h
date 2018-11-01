// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomsManager.generated.h"

USTRUCT()
struct FSymptom
{
	GENERATED_BODY()
	UFUNCTION()
	FSymptom(AActor* SymptomActor) : TaggedActor(SymptomActor)
	{
		// 10 seconds for now; need to update with actual symptom->timeDuration
		// once function table is mapped
		Duration = FTimespan::FTimespan(0, 0, 10);
	}

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
	void AddActorsWithActiveSymptoms();

	// Removes actors with an expired symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	void RemoveExpiredSymptoms();

	// Tags an actor with a Symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
	void TagActorWithSymptom(AActor* Actor, FName SymptomTag); // may change
};
