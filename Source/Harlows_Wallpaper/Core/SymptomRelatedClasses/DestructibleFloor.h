// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleComponent.h"
#include "ConstructorHelpers.h"
#include "EngineUtils.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DestructibleFloor.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ADestructibleFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructibleFloor();

	UFUNCTION(BlueprintCallable)
		void FallToRuin();

	UFUNCTION(BlueprintCallable)
		void StartSymptom(float StartRestoreTime, float ForceRestoreTime);

	UFUNCTION()
		void StartChecking();

	UFUNCTION(BlueprintCallable)
		void ForceRestore();

	UFUNCTION()
		void Restore();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UBoxComponent * BoxComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UDestructibleComponent* DestructibleComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UStaticMeshComponent* Failsafe;

	UPROPERTY()
		FVector Position;

	UPROPERTY()
		bool ShouldCheck = false;

	UPROPERTY()
		TMap<FString, FTransform> Map;

	UPROPERTY()
		FName TagName = "fall";

	FTimerHandle TimerHandle;
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;
	FTimerHandle TimerHandle3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	void Tick(float DeltaTime);

	void Check();

};
