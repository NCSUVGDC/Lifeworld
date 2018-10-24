// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomsManager.h"


// Sets default values
ASymptomsManager::ASymptomsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASymptomsManager::BeginPlay()
{
	Super::BeginPlay();
}

void ASymptomsManager::RemoveSymptomFromActor(AActor * TaggedActor)
{
	// TODO
}

// Called every frame
void ASymptomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Scan for new tags
	// create a struct for each
	// add to active symptoms array
	// add to active symptoms array

}

void ASymptomsManager::AddActorWithActiveSymptom(const AActor * SymptomActor)
{
	// TODO
}

void ASymptomsManager::RemoveExpiredSymptoms()
{
	for (FSymptom s : SymptomActors)
	{
		if (s.Duration.GetTotalSeconds() < 0.0)
			SymptomActors.Remove(s);
	}
}

void ASymptomsManager::TagActorWithSymptom(AActor * Actor, ESymptomTypes SymptomType)
{
	// TODO
}

