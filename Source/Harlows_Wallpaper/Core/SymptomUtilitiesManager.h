// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"

#define MAX_SYMPTOMS 1

/**
 * Contains symptom effects and utility properties
 */
class HARLOWS_WALLPAPER_API SymptomUtilitiesManager
{
public:
	SymptomUtilitiesManager();
	~SymptomUtilitiesManager();

	// function pointer taking a single Actor argument
	typedef void(SymptomUtilitiesManager::*SingleActorFuncPtr)(AActor*);

	// symptom functions array
	UPROPERTY()
	const SingleActorFuncPtr SymptomFunctions[MAX_SYMPTOMS] = { &SymptomUtilitiesManager::Flee };
	
	// index of symptom effect
	UPROPERTY()
	static TMap<FName, int32> SymptomFunctionIndexes;
	
	// duration of each symptom map
	UPROPERTY()
	static TMap<FName, FTimespan> SymptomDurations;

	// display symptom effect
	UFUNCTION()
	void DisplaySymptom(int32 SymptomIndex, AActor* SymptomActor);

	// get singleton instance
	UFUNCTION()
	static SymptomUtilitiesManager* GetInstance();
	
	/* Symptom functions */

	// Flee from player
	UFUNCTION()
	void Flee(AActor* SymptomActor);

private:
	UPROPERTY()
	// use to access symptoms utilities
	static SymptomUtilitiesManager* Singleton;

	/*UFUNCTION()
	static TMap<FName, SingleActorFuncPtr> InitSymptomFunctions();*/
	UFUNCTION()
	static TMap<FName, int32> InitSymptomFunctionIndexes();

	UFUNCTION()
	static TMap<FName, FTimespan> InitSymptomDurations();
};
