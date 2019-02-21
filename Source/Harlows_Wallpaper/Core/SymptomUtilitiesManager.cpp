// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

#define FLEE 0
#define DOUBLETAKEPHANTOM 2

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

	// Double-Take phantom
	SymptomContainers.Add(FName(TEXT("Symptoms.DoubleTakePhantom")), FSymptomDetails(DOUBLETAKEPHANTOM, FTimespan(0, 0, 20)));
	
	return SymptomContainers;
}
