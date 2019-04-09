// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SymptomsManager.h"
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


private:

	int tickCount = 0;

	void SpawnPhantom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Phantom", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* specMesh;

	//Reference to the player, allows for quick access for phantom to check if it has been spotted
	AActor * player;

	//Tells if the phantom has been spotted by the player
	bool isSpotted = false;

	//Stores the time that the phantom was spotted by the player.
	//Allows Phantom to go away eventually, even if player never actually looks directly at it.
	//Prevents player from constantly watching it from the corner of their eye
	float timeSpotted = 0.0f;

	//Stores the amount of time the phantom is supposed to run. In the event that the SymptomManager stops updating this symptom,
	// and the phantom was never spotted, we don't want the phantom just sitting there without anyway to update.
	//As such, phantom stores its own time, equal to the max duration of the symptom, so that it can automatically retire itself if needed
	float symptomTime = 0.0f;

	//Tells if the symptom is currently Running. 
	//Switches to true when phantom becomes visible
	//Switches to false when the phantom disappears
	bool isRunning = false;

	FTimerHandle endSymptomTimer;

	float timeToSpawnAt = 20.0f;

};
