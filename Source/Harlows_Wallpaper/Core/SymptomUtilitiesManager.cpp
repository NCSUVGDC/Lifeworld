// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomUtilitiesManager.h"

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
	SymptomContainers.Add(FName(TEXT("Symptoms.SizePerception")), FSymptomDetails(SizePerception, FTimespan(0, 0, 20)));

	// voices
	SymptomContainers.Add(FName(TEXT("Symptoms.Voices")), FSymptomDetails(Voices, FTimespan(0, 4, 0)));

	// double-take
	SymptomContainers.Add(FName(TEXT("Symptoms.DoubleTake")), FSymptomDetails(DoubleTake, FTimespan(0, 0, 10)));

	// Warping Walls
	SymptomContainers.Add(FName(TEXT("Symptoms.WarpingWalls")), FSymptomDetails(WarpingWalls, FTimespan(0, 0, 20)));

	// Phantom
	SymptomContainers.Add(FName(TEXT("Symptoms.Phantom")), FSymptomDetails(Phantom, FTimespan(0, 0, 10)));

	// BackIsTurned
	SymptomContainers.Add(FName(TEXT("Symptoms.BackIsTurned")), FSymptomDetails(BackIsTurned, FTimespan(0, 0, 30)));

	// Lens Flaring (potentially cut for demo)
	SymptomContainers.Add(FName(TEXT("Symptoms.LensFlaring")), FSymptomDetails(LensFlaring, FTimespan(0, 0, 5)));

	// Breathe In (potentially cut)
	SymptomContainers.Add(FName(TEXT("Symptoms.BreatheIn")), FSymptomDetails(BreatheIn, FTimespan(0, 0, 5)));

	// Falling Floor
	SymptomContainers.Add(FName(TEXT("Symptoms.FallingFloor")), FSymptomDetails(FallingFloor, FTimespan(0, 0, 20)));
	
	//Falling Floor
	SymptomContainers.Add(FName(TEXT("Symptoms.Falling_Floor")), FSymptomDetails(1, FTimespan(0, 0, 1)));

	return SymptomContainers;
}
