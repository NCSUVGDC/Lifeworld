// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

#define FLEE 0
#define FALLING_FLOOR 9

USymptomUtilitiesManager::USymptomUtilitiesManager()
{
}

USymptomUtilitiesManager::~USymptomUtilitiesManager()
{
}

// initialize details for each symptom
TMap<FName, FSymptomDetails> USymptomUtilitiesManager::SymptomDetails = InitSymptomDetails();

TMap<FName, FSymptomDetails> USymptomUtilitiesManager::InitSymptomDetails()
{
	TMap<FName, FSymptomDetails> SymptomContainers;
	
	// flee
	SymptomContainers.Add(FName(TEXT("Symptoms.Flee")), FSymptomDetails(FLEE, FTimespan(0, 0, 5)));
	
	//Falling Floor
	SymptomContainers.Add(FName(TEXT("Symptoms.Falling_Floor")), FSymptomDetails(1, FTimespan(0, 0, 1)));

	return SymptomContainers;
}
