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

	// Get total elapsed days
	UFUNCTION(BlueprintCallable, Category = "TimeSystem")
	int32 GetElapsedDays();

	UFUNCTION(BlueprintCallable, Category = "TimeSystem")
	// Get current minute
	int32 CurrentMinute();
	
	UFUNCTION(BlueprintCallable, Category = "TimeSystem")
	// Get current hour
	int32 CurrentHour();

	UFUNCTION(BlueprintCallable, Category = "TimeSystem")
	// Get current second
	int32 CurrentSecond();

private:
	// amount of in-game time passed
	FTimespan CurrentTime;
	// flag for enabling debug output
	bool EnableDebug;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// rate at which time progresses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Rate = 1.0f;
};