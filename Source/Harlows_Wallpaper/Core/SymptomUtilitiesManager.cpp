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
TMap<FName, FSymptomDetail> USymptomUtilitiesManager::SymptomDetails = InitSymptomDetails();

TMap<FName, FSymptomDetail> USymptomUtilitiesManager::InitSymptomDetails()
{
	TMap<FName, FSymptomDetail> SymptomContainers;
	
	// flee
	SymptomContainers.Add(FName(TEXT("Symptoms.Flee")), FSymptomDetail(FLEE, FTimespan(0, 0, 5)));
	
	return SymptomContainers;
	return SymptomContainers;
}
