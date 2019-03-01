// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DestructibleFloor.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ADestructibleFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleFloor();

	UFUNCTION()
		void FallToRuin();

	UFUNCTION()
		void Restore();

	UPROPERTY(EditAnywhere)
		UDestructibleComponent* DestructibleComponent;

	UPROPERTY()
		FVector position;

	UPROPERTY()
		bool destroyed;

	UPROPERTY()
		TArray<AActor*> fallers;

	UPROPERTY()
		int size;

	UPROPERTY()
		TArray<FTransform> previousTrans;

	UPROPERTY()
		float elapsedTime;

	UPROPERTY()
		float restoreStart = 5;

	UPROPERTY()
		float forceRestore = 10;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	void Tick(float DeltaTime);

	// Set if the floor should tick
	void setTick(bool b);

};
