// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	void setPlayer(AActor *actor);


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* specMesh;

	AActor * player;

	bool isSeen = false;

	float timeSpotted = 0.0f;


};
