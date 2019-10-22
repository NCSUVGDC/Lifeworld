// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SymptomsManager.h"
#include "Core/TimeSystem.h"
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
	bool SelectObject();

	//Update called once per frame by SymptomManager
	void Update();

	UPROPERTY(EditAnywhere)
		class ASymptomsManager* SymptomManager;

	UPROPERTY(EditAnywhere)
		class ATimeSystem* TimeSystem;

private:

	//Ends the symptom, immediately returns object to resting place
	void EndSymptom();

	//# of ticks since DoubleTake started running
	int tickCount = 0;

	// Array of actors that can be possess by double take function
//	UPROPERTY()
	TArray<AActor*> DoubleTakeActors;

	//Reference to object
	AActor * object;
	//Refence to player
	AActor * player;

	//X distance of object from original location
	float ddX;
	//Y distance of object from original location
	float ddY;

	//Number of movement iterations
	int iterationCount = 0;

	//Starting location + rotation of object
	FVector startLoc = FVector::ZeroVector;
	FRotator startRot = FRotator::ZeroRotator;

	//Tells if the symptom is currently Running. 
	//Switches to true when an object has been chosen to move under this Symptom
	//Switches to false when the object moving returns to resting position
	bool isRunning = false;

	//Tells if the object has been spotted
	bool isSpotted = false;

	//Time (in seconds) in game time when the DoubleTake should spawn
	float timeToSpawnAt = 105.f;
};