// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowPawn.h"
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
