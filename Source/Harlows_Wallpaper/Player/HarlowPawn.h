// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Phantom.h"
#include "PossessedObject.h"
#include "GameFramework/Pawn.h"
#include "HarlowPawn.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AHarlowPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHarlowPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	/**
	* create phantom to haunt player at the edge
	* of their vision.
	*/
	UFUNCTION()
		void ImposePhantom();

	/**
	* forces actor to dart away from player's
	* line-of-sight. actor returns to normal
	* when player looks at it within a certain
	* radius.
	*/
	UFUNCTION()
		void ImposeDoubleTake();

private:

	// Array of actors that can be possess by double take function
	UPROPERTY()
		TArray<AActor*> DoubleTakeActors;

	UPROPERTY()
		APhantom* phantom;
};
