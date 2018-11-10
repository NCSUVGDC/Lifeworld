// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomUtilitiesManager.h"
#include "SymptomsManager.generated.h"

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

	FSymptom(AActor* TaggedActor, FTimespan SymptomDuration, int32 EffectIndex)
		: SymptomActor(TaggedActor), Duration(SymptomDuration), SymptomEffectIndex(EffectIndex) {}
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
	// Array of actors with active Symptoms
	UPROPERTY()
		TArray<FSymptom> SymptomActors;

	UFUNCTION()
		void DisplayActiveSymptoms();

	UFUNCTION()
		void ExpireActiveSymptoms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ASymptomsManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Adds an Actor with an active symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
		void AddActorsWithActiveSymptoms();

	// Removes actors with an expired symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
		void RemoveExpiredSymptoms();

	// Tags an actor with a Symptom
	UFUNCTION(BlueprintCallable, Category = "SymptomManager")
		void TagActorWithSymptom(AActor* Actor, FName SymptomTag);
};