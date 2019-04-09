// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoubleTake.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ADoubleTake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoubleTake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set refence to what object should be affected by this symptom
	void setObject(AActor * obj);

	//Set refence to the player for easy access when calculating dot products
	void setPlayer(AActor * ply);

	//Selects an object to possess
	void SelectObject();

	//Update called once per frame by SymptomManager
	void Update();

private:
	
	// Array of actors that can be possess by double take function
//	UPROPERTY()
	TArray<AActor*> DoubleTakeActors;
	
	//Reference to object
	AActor * object;
	//Refence to player
	AActor * player;
	//Tells if the object has been spotted
	bool isSpotted = false;
	//X distance of object from original location
	float ddX;
	//Y distance of object from original location
	float ddY;
	//Number of iteration movements
	int iterationCount = 0;

	//Starting location + rotation of object
	FVector startLoc = FVector::ZeroVector;
	FRotator startRot = FRotator::ZeroRotator;

	//Stores the amount of time the phantom is supposed to run. In the event that the SymptomManager stops updating this symptom,
	// and the phantom was never spotted, we don't want the phantom just sitting there without anyway to update.
	//As such, phantom stores its own time, equal to the max duration of the symptom, so that it can automatically retire itself if needed
	float symptomTime = 0.0f;

	//Tells if the symptom is currently Running. 
	//Switches to true when an object has been chosen to move under this Symptom
	//Switches to false when the object moving returns to resting position
	bool isRunning = false;

	int tickCount = 0;
};
