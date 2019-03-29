#include "SizePerceptionActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

SizePerceptionActor::SizePerceptionActor()
{
}


SizePerceptionActor::~SizePerceptionActor()
{
}

AStaticMeshActor SizePerceptionActor::Select()
{
	
}

void SizePerceptionActor::BeginPlay() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));

	TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// Add actors to symptom array
	for (AActor* SymptomActor : FoundActors)
	{
		// Check for active symptoms
		if (SymptomActor->Tags.Num() != 0)
		{
			// Loop backwards over the list so we can safely remove tags as we iterate
			for (int SymptomIdx = SymptomActor->Tags.Num() - 1; SymptomIdx >= 0; SymptomIdx--)
			{
				FName& Symptom = SymptomActor->Tags[SymptomIdx];

				// Check if the tag says it can be scaled with SizePerception
				if (Symptom == FName("SizePerception"))
				{
					SizePerceptionActors.Add(SymptomActor);
				}
			}
		}

		
	}


}