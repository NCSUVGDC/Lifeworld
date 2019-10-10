// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Public/TimerManager.h"
#include "CoreMinimal.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "GameFramework/Actor.h"
#include "Voices.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AVoices : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVoices();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
        AHarlowPawn* playerPawn;
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
	UFUNCTION()
		void StopWhisperTimer();
};
