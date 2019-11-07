// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SymptomsManager.h"
#include "Core/TimeSystem.h"
#include "Phantom.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API APhantom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APhantom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Prepare phantom to run the symptom
	void SetPlayer(AActor *actor);

	UPROPERTY(EditAnywhere)
		class ATimeSystem* TimeSystem;

	/* Activate the phantom symptom. The Phantom will find a location where the player is not looking and spawn there

	*Kill Time:				The total time this symptom should last. 0 = unlimited (until Phantom is spotted)
	*PeripheralViewBound:	how far the phantom is allowed to appear into the player's F.O.V before the timer for it to disappear begins (1 = direct line of sight)
	*DirectViewBound:		how far the phantom is allowed to appear into the player's F.O.V before the phantom is forced to disappear (1 = direct line of sight)
	*ViewTimeAllowed:		Time after entering peripheral view before the phantom is forced to disappear. 0 = unlimited

	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		void ActivatePhantom(int KillTime = 0, float PeripheralViewBound = 0.62f, float DirectViewBound = 0.75f, float ViewTimeAllowed = 0.3f);

	/* Activate the phantom symptom and have it appear at a specific location

	*spawnLoc: the location the phantom should spawn at
	*Kill Time:				The total time this symptom should last. 0 = unlimited (until Phantom is spotted)
	*PeripheralViewBound:	how far the phantom is allowed to appear into the player's F.O.V before the timer for it to disappear begins (1 = direct line of sight)
	*DirectViewBound:		how far the phantom is allowed to appear into the player's F.O.V before the phantom is forced to disappear (1 = direct line of sight)
	*ViewTimeAllowed:		Time after entering peripheral view before the phantom is forced to disappear. 0 = unlimited

	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		void SpawnPhantomWithLocation(FVector spawnLoc, int KillTime = 0, float PeripheralViewBound = 0.62f, float DirectViewBound = 0.75f, float ViewTimeAllowed = 0.3f);

	/* Immediately ends the symptom */
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		void EndSymptom();



private:

	// Phantom updates states per tick it is active
	void Update();

	//Find a location outside of the player's view range where phantom can spawn
	void FindPhantomSpawn();

	//spawn the phantom in the given location and set it to enabled
	void SpawnPhantom(FVector spawnLoc);

	//Reference to the player, allows for quick access for phantom to check if it has been spotted
	AActor * player;

	//Tells if the phantom has been spotted by the player (has passed peripheral bound)
	bool isSpotted = false;

	//Stores the time that the phantom was spotted by the player.
	//Allows Phantom to go away eventually, even if player never actually looks directly at it.
	//Prevents player from constantly watching it from the corner of their eye
	float timeSpotted = 0.0f;

	//Tells if the symptom is currently Running. 
	//Switches to true when phantom becomes visible
	//Switches to false when the phantom disappears
	bool isRunning = false;

	//time after spawn the phantom should automatically expire
	int _KillTime = 0;

	//time the phantom last attempted to spawn
	float timeOfLastSpawnAttempt = 0.0f;

	//boundary of the player's peripheral view
	float _PeripheralViewBound = 0.0f;
	//boundary of the player's direct line of sight
	float _DirectViewBound = 0.0f;

	//time allowed after entering peripheral view before the phantom should expire
	float _ViewTimeAllowed = 0.0f;

};
