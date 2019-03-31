// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "SizePerceptionActor.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ASizePerceptionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASizePerceptionActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Locates the actor used in SizePerception
	AActor* Select();

	// Brings a resized actor back to normal
	void ScaleOriginal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Array of actors that can be scaled by Size Perception
	UPROPERTY()
		TArray<AActor*> SizePerceptionActors;

	// Array of dot products between each SizePerceptionActor and the player
	UPROPERTY()
		TArray<float> DotProducts;

	// Array of actors that can be scaled by Size Perception
	UPROPERTY()
		TArray<AActor*> FinalSelectionRandomizer;

	// Player pawn
	UPROPERTY()
		APawn* LifeWorldPlayer;

	// Player camera
	UPROPERTY()
		USceneComponent* LifeWorldCamera;

	// Distance of SelectedActor
	UPROPERTY()
		float SADistance;

	// Current sizing scale
	UPROPERTY()
		float scaleSize;
	
	// Proceed with SizePerception activation
	UPROPERTY()
		bool proceed;

	// Recent scale compared against in resizing to prevent regression
	UPROPERTY()
		float tempScale;

	// Determines if scaling is going up or down
	UPROPERTY()
		bool initScaleUp;
};
