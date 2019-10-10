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

	// Phantom updates states per call by SymptomManager
	void Update();

	void EndSymptom();

	UPROPERTY(EditAnywhere)
		class ASymptomsManager* SymptomManager;

	UPROPERTY(EditAnywhere)
		class ATimeSystem* TimeSystem;


private:

	int tickCount = 0;

	bool FindPhantomSpawn();

	void SpawnPhantom(FVector spawnLoc);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phantom", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* specMesh;

	//Reference to the player, allows for quick access for phantom to check if it has been spotted
	AActor * player;

	//Tells if the phantom has been spotted by the player
	bool isSpotted = false;

	//Stores the time that the phantom was spotted by the player.
	//Allows Phantom to go away eventually, even if player never actually looks directly at it.
	//Prevents player from constantly watching it from the corner of their eye
	int timeSpotted = 0;

	//Tells if the symptom is currently Running. 
	//Switches to true when phantom becomes visible
	//Switches to false when the phantom disappears
	bool isRunning = false;

	//Time (in seconds) in game time when the phantom should spawn
	float timeToSpawnAt = 165.f;

};
