// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

#define FLEE 0

SymptomUtilitiesManager::SymptomUtilitiesManager()
{
}

SymptomUtilitiesManager::~SymptomUtilitiesManager()
{
	delete Singleton;
}

// Setup singleton
SymptomUtilitiesManager* SymptomUtilitiesManager::Singleton;
// Initialize static tables
TMap<FName, int32> SymptomUtilitiesManager::SymptomFunctionIndexes = SymptomUtilitiesManager::InitSymptomFunctionIndexes();
TMap<FName, FTimespan> SymptomUtilitiesManager::SymptomDurations = SymptomUtilitiesManager::InitSymptomDurations();

void SymptomUtilitiesManager::Flee(AActor * SymptomActor)
{
	if (GEngine)
	{
		// Test if we actually get an actor
		FString DebugMsg = FString::Printf(TEXT("Running Flee Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}
	UE_LOG(LogTemp, Warning, TEXT("Running Flee Symptom on Actor %s"), *SymptomActor->GetName());
	// TODO: actor flees from player's sight
}

void SymptomUtilitiesManager::DisplaySymptom(int32 SymptomIndex, AActor* SymptomActor)
{
	// shouldn't have to perform bounds check as this will only be private
	(this->* (SymptomFunctions[SymptomIndex]))(SymptomActor);
}

SymptomUtilitiesManager* SymptomUtilitiesManager::GetInstance()
{
	if (!Singleton)
		Singleton = new SymptomUtilitiesManager();
	
	return Singleton;
}

TMap<FName, FTimespan> SymptomUtilitiesManager::InitSymptomDurations()
{
	TMap<FName, FTimespan> DurationsMap;
	
	// Add durations to durations map
	DurationsMap.Add(FName(TEXT("Symptoms.Flee")), FTimespan(0, 0, 10)); // per Jackson, will at most be only a few seconds

	return DurationsMap;
}

TMap<FName, int32> SymptomUtilitiesManager::InitSymptomFunctionIndexes()
{
	TMap<FName, int32> IndexesMap;

	IndexesMap.Add(FName(TEXT("Symptoms.Flee")), FLEE);

	return IndexesMap;
}
