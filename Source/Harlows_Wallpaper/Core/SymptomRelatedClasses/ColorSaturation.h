// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorSaturation.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AColorSaturation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColorSaturation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
		Set the color saturation of the player's camera

		*Saturation:	The saturation settings for the camera
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		FVector4 SetColorSaturation(FVector4 Saturation);

private:
	// Player pawn
	UPROPERTY()
		APawn *lifeworldPlayer;

	// Camera component of player pawn
	UPROPERTY()
		USceneComponent *lifeworldCamera;

};
