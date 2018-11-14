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

// Called every frame
void ASymptomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorsWithActiveSymptoms();
	DisplayActiveSymptoms();
	ExpireActiveSymptoms();
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
			for (FName Symptom : Tags)
			{
				FTimespan Duration = *(SymptomUtilitiesManager::SymptomDurations.Find(Symptom));
				int32 Effect = *(SymptomUtilitiesManager::SymptomFunctionIndexes.Find(Symptom));

				// add symptom and remove tags
				SymptomActors.Add(FSymptom(SymptomActor, Duration, Effect));
				UE_LOG(LogTemp, Warning, TEXT("Added %s to active symptoms"), *(SymptomActor->GetName()));
				SymptomActor->Tags.Empty();
			}
		}
	}
}

void ASymptomsManager::RemoveExpiredSymptoms()
{
	int32 Length = SymptomActors.Num();
	for (int i = 0; i < Length; ++i)
	{
		FSymptom Symptom = SymptomActors[i]; // cache for updating
		if (Symptom.Duration.GetTotalSeconds() <= 0.0)
		{
			SymptomActors.Remove(Symptom); // remove expired symptom
			UE_LOG(LogTemp, Warning, TEXT("%s with Symptom ID %d expired"), *(Symptom.SymptomActor->GetName()), Symptom.SymptomEffectIndex);
			Length = SymptomActors.Num();  // update length
		}
	}
}

void ASymptomsManager::TagActorWithSymptom(AActor* ActorToTag, FName SymptomTag)
{
	ActorToTag->Tags.Add(SymptomTag);
}

void ASymptomsManager::DisplayActiveSymptoms()
{
	for (FSymptom SymptomActor : SymptomActors)
	{
		SymptomUtilitiesManager::GetInstance()->DisplaySymptom(SymptomActor.SymptomEffectIndex, SymptomActor.SymptomActor);
	}
}

void ASymptomsManager::ExpireActiveSymptoms()
{
	static FTimespan Decrement = FTimespan(0, 0, 1); // decrement a second each expiration
	for (int i = 0; i < SymptomActors.Num(); ++i)
	{
		FSymptom* Symptom = &SymptomActors[i];
		Symptom->Duration -= Decrement;
		UE_LOG(LogTemp, Warning, TEXT("%f seconds remaining for active symptom in %s"), Symptom->Duration.GetTotalSeconds(), *(Symptom->SymptomActor->GetName()));
	}
}

