// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SomethingsChanged.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ASomethingsChanged : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASomethingsChanged();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
		Changes an actor's material into a specified material

		*Object:	The changed actor
		*Material:	The new material

		*Return:	Original material interface, null if change doesn't occur
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		UMaterialInterface* MaterialChanged(AActor *object, UMaterialInterface *material);

	/*
		Changes an actor's location to a specified location

		*Object:	The changed actor
		*Location:	The new location

		*Return:	Original location of object, zero vector if change doesn't occur
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		FVector LocationChanged(AActor *object, FVector location);

	/*
		Swap two actors' materials

		*Object1:	One of the changed actors
		*Object2:	Other one of the changed actors

		*Return:	Array of actors in order of objects, null array if no change
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		TArray<AActor*> MaterialsSwap(AActor *object0, AActor *object1);

	/*
		Swap two actors' locations

		*Object1:	One of the changed actors
		*Object2:	Other one of the changed actors

		*Return:	Array of original locations in order of objects, null array if no change
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		TArray<FVector> LocationsSwap(AActor *object0, AActor *object1);
	
private:
	// Array of actors that can be accessed by symptom
	UPROPERTY()
		TArray<AActor*> validActors;

	// Player pawn
	UPROPERTY()
		APawn *lifeworldPlayer;

	// Camera component of player pawn
	UPROPERTY()
		USceneComponent *lifeworldCamera;

};
