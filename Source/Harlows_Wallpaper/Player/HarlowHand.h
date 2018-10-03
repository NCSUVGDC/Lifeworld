// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarlowPawn.h"
#include "HarlowHand.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AHarlowHand : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	/** Cached to make accessing the user's input easier */
	UInputComponent* PlayerInput;

public:	
	AHarlowHand();
	virtual void Tick(float DeltaTime) override;

	/** 
	 * The HarlowPawn controlling this Hand
	 * Actual assignment is currently done in Blueprints as that is where hands
	 * are spawned. (If you're curious why, we started off with a copy of the 
	 * UVRF pawn and hands to get the ball rolling faster.)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player", META = (ExposeOnSpawn="True"))
		AHarlowPawn* OwningPawn;



	
};
