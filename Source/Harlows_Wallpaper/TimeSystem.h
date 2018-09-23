// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeSystem.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ATimeSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeSystem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// time span for progressing in-game time
	FTimespan Accumulator;
	// amount of in-game time passed
	FTimespan CurrentTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// print current time as debug text
	UFUNCTION(BlueprintCallable, Category="TimeSystem")
		void DebugDisplayTime();

	// Get total elapsed days
	UFUNCTION(BlueprintCallable, Category = "TimeSystem")
	int32 GetElapsedDays();

	// private function to format time for display
	/*UFUNCTION(BlueprintCallable)
	FString FormatTime();*/

	// rate at which time progresses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float Rate;



};