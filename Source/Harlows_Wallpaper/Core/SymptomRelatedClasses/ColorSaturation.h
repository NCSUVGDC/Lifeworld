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

		*Return:		The original saturation
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		FVector4 SetColorSaturation(FVector4 NewSaturation);

	/*
	//	Set the color saturation of the player's camera with a transition

	//	*Saturation:	The saturation settings for the camera
	//	*Seconds:		The duration of the transition

	//	*Return:		The original saturation
	*/
	//UFUNCTION(BlueprintCallable, Category = "Symptoms")
	//	FVector4 TransitionColorSaturation(FVector4 NewSaturation, float Seconds);

private:
	// Player pawn
	UPROPERTY()
		APawn *lifeworldPlayer;

	// Camera component of player pawn
	UPROPERTY()
		USceneComponent *lifeworldCamera;

	// The current saturation values in use
	FVector4 Saturation;

	//// Lerp saturation for transitions
	//FVector4 TargetSaturation;

	//// If the saturation is current in transition
	//bool bInTransition;

	//// Transition and elapsed time
	//float TransitionTime;
	//float ElapsedTime;
};
