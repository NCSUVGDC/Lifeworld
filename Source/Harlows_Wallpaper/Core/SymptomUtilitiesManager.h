// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "SymptomUtilitiesManager.generated.h"

//#define MAX_SYMPTOMS 1

// Container defining a Symptom's effect and duration
USTRUCT()
struct FSymptomDetails
{
	GENERATED_BODY()
	
	// index of symptom effect
	UPROPERTY()
	int32 SymptomEffectIndex;

	// duration of each symptom
	UPROPERTY()
	FTimespan SymptomDuration;

	// details constructor (with default values for testing purposes)
	FSymptomDetails(int32 FunctionIndex = 0, FTimespan Duration = FTimespan(0, 0, 0)) : SymptomEffectIndex(FunctionIndex), SymptomDuration(Duration) {}
};

/**
 * Contains symptom effects and utility properties
 */
UCLASS()
class HARLOWS_WALLPAPER_API USymptomUtilitiesManager : public UObject
{
	GENERATED_BODY()

public:
	USymptomUtilitiesManager();
	~USymptomUtilitiesManager();

	// Details for each symptom; names are in the format 'Symptom.blahblahblah'
	static TMap<FName, FSymptomDetails> SymptomDetails;

private:
	// Initializes symptom details and assigns them to the static SymptomDetails map
	static TMap<FName, FSymptomDetails> InitSymptomDetails();
};
