// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowHand.h"


// Sets default values
AHarlowHand::AHarlowHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarlowHand::BeginPlay()
{
	Super::BeginPlay();
	
	if (OwningPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AHarlowHand spawned with no owning pawn!"));
	}
	else
	{
		PlayerInput = OwningPawn->InputComponent;

		if (PlayerInput == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Player pawn missing input component!"));
		}
	}

}

// Called every frame
void AHarlowHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

