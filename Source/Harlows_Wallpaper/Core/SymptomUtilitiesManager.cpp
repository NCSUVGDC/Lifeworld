// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

#define SIZE_PERCEPTION 0

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
	
	// size perception
	SymptomContainers.Add(FName(TEXT("Symptoms.SizePerception")), FSymptomDetails(SizePerception, FTimespan(0, 0, 5)));

	// voices
	SymptomContainers.Add(FName(TEXT("Symptoms.Voices")), FSymptomDetails(Voices, FTimespan(0, 0, 5)));

	// double-take
	SymptomContainers.Add(FName(TEXT("Symptoms.DoubleTake")), FSymptomDetails(DoubleTake, FTimespan(0, 0, 5)));

	// Warping Walls
	SymptomContainers.Add(FName(TEXT("Symptoms.WarpingWalls")), FSymptomDetails(WarpingWalls, FTimespan(0, 0, 5)));

	// Phantom
	SymptomContainers.Add(FName(TEXT("Symptoms.Phantom")), FSymptomDetails(Phantom, FTimespan(0, 0, 5)));

	// BackIsTurned
	SymptomContainers.Add(FName(TEXT("Symptoms.BackIsTurned")), FSymptomDetails(BackIsTurned, FTimespan(0, 0, 5)));

	// Lens Flaring
	SymptomContainers.Add(FName(TEXT("Symptoms.LensFlaring")), FSymptomDetails(LensFlaring, FTimespan(0, 0, 5)));

	// Breathe In
	SymptomContainers.Add(FName(TEXT("Symptoms.BreatheIn")), FSymptomDetails(BreatheIn, FTimespan(0, 0, 5)));

	// Falling Floor
	SymptomContainers.Add(FName(TEXT("Symptoms.FallingFloor")), FSymptomDetails(FallingFloor, FTimespan(0, 0, 5)));
	
	return SymptomContainers;
}
