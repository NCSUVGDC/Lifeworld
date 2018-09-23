// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarlowHand.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AHarlowHand : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:	
	AHarlowHand();
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION(BlueprintCallable, Category = "Harlow")
		bool */
	
};
