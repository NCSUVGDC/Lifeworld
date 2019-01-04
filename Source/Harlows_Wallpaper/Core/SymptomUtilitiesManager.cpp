// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

#define FLEE 0

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
	SymptomContainers.Add(FName(TEXT("Symptom.Flee")), FSymptomDetails(FLEE, FTimespan(0, 0, 5)));
	
	return SymptomContainers;
}
