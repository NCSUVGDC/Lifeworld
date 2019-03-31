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

	void setObject(AActor * obj);

	void setPlayer(AActor * ply);

	void DoASpoopyThing();

private:

	AActor * object;

	AActor * player;

	bool isSpotted = false;

	float ddX;
	float ddY;

	float timeSpotted;

	FVector startLoc = FVector::ZeroVector;
	FRotator startRot = FRotator::ZeroRotator;


};
