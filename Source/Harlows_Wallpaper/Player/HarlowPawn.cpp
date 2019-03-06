// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"


// Sets default values
AHarlowPawn::AHarlowPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarlowPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

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

				//check if the tag says it can be moved by DoubleTakeSymptom
				if (Symptom == FName("Possess"))
				{
					DoubleTakeActors.Add(SymptomActor);
				}
			}
		}
	}

	//Get reference to phantom
	//Find the phantom that exists in the world and store a reference to it
	TArray<AActor*> GhostsNStuff;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);
	//	FString NumStr = FString::FromInt(GhostsNStuff.Num());
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *NumStr);

	//If we have the one phantom found, store the reference at index 0 of TArray
	if (GhostsNStuff.Num() == 1)
	{
		//Call the function in 5 seconds
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 5.0f, false);
	}
	else
	{
		//Call the function in 5 seconds
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposeDoubleTake, 5.0f, false);
	}
	
}

// Called every frame
void AHarlowPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHarlowPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHarlowPawn::ImposeDoubleTake()
{
	bool foundActor = false;

	// Add actors to symptom array
	for (AActor* Object : DoubleTakeActors)
	{
		float DotProd = GetDotProductTo(Object);
		if (DotProd >= 0.4 && DotProd < 0.6)
		{
			//Spawn a new PossessObject and tie player and object to it
			APossessedObject* pj = (APossessedObject*)GetWorld()->SpawnActor(APossessedObject::StaticClass());

			pj->setObject(Object);
			pj->setPlayer( this );
			pj->DoASpoopyThing();
			foundActor = true;
			break;
		}
		if (!foundActor)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to run Double-Take"));
			//Try again in 3 seconds
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposeDoubleTake, 3.0f, false);
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Running Double-Take!"));
		}
	}

}

void AHarlowPawn::ImposePhantom()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Running Phantom"));

	//Get reference to phantom
	//Find the phantom that exists in the world and store a reference to it
	TArray<AActor*> GhostsNStuff;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);
	//	FString NumStr = FString::FromInt(GhostsNStuff.Num());
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *NumStr);

	//If we have the one phantom found, store the reference at index 0 of TArray
	if (GhostsNStuff.Num() == 1)
	{
		phantom = (APhantom*)GhostsNStuff[0];
		phantom->SetActorHiddenInGame(false);
		phantom->SetPlayer(this);
		FVector ghostLoc = this->GetActorLocation();
		ghostLoc += FVector(-50, 300, 0);
		phantom->SetActorLocation(ghostLoc);
		phantom->SetActorTickEnabled(true);
	}

}

