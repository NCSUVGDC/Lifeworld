// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PossessedObject.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API APossessedObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APossessedObject();

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

	//Do a spook
	void DoASpoopyThing();

	//Update PossessedObject per call of SymptomManager
	void Update();

private:
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
};
