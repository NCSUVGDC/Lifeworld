// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomsManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"


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
	// TODO: May not be needed (see below)
}

// Called every frame
void ASymptomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorsWithActiveSymptoms();
	RemoveExpiredSymptoms();
}

void ASymptomsManager::AddActorsWithActiveSymptoms()
{
	// Scan for new tags
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// Add actor to symptom array
	for (AActor* SymptomActor : FoundActors)
	{
		TArray<FName> Tags = SymptomActor->Tags;
		// check for active symptoms
		if (Tags.Num() != 0)
		{
			// add symptoms and remove tags
			SymptomActors.Add(FSymptom(SymptomActor));
			SymptomActor->Tags.Empty();
		}
	}
}

void ASymptomsManager::RemoveExpiredSymptoms()
{
	for (FSymptom s : SymptomActors)
	{
		if (s.Duration.GetTotalSeconds() < 0.0)
			SymptomActors.Remove(s);
	}
}

void ASymptomsManager::TagActorWithSymptom(AActor * Actor, FName SymptomTag)
{
	// TODO
}

