// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreathingWalls.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ABreathingWalls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreathingWalls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
		bool voicesActivated;
	UPROPERTY()
		FTimerHandle voiceTimerHandle;
	UPROPERTY()
		FTimerHandle whisperTimerHandle;
	UPROPERTY()
		FTimerHandle stopWhisperTimerHandle;
	UPROPERTY()
		FTimerHandle gibberishTimerHandle;
	UPROPERTY()
		FTimerHandle footstepsTimerHandle;
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BrushMat"))
		void SetBrushMat(AActor *brush);
	
};
