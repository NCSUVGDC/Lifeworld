// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeSystem.h"
#include "Engine.h"

// Sets default values
ATimeSystem::ATimeSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimeSystem::BeginPlay()
{
	Super::BeginPlay();
	
	// toggle debug function based on in-game settings
	GConfig->GetBool(TEXT("/Script/Harlows_Wallpaper.CustomGameSettings"), TEXT("bEnableDebugMessages"), EnableDebug, GGameIni);
}

// Called every frame
void ATimeSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Update current time
	FTimespan TimePassed = FTimespan::FromSeconds(DeltaTime * Rate);
	CurrentTime += TimePassed;
	
	// Log debug output if enabled
	if (EnableDebug)
	{
		if (GEngine)
		{
			FString DebugMsg = FString::Printf(TEXT("ElapsedTime: %d:%d:%d\nElapsedDays: %d"), CurrentTime.GetHours(), CurrentTime.GetMinutes(), CurrentTime.GetSeconds(), GetElapsedDays());
			GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
		}

		UE_LOG(LogTemp, Warning, TEXT("ElapsedTime: %d:%d:%d\nElapsedDays: %d"), CurrentTime.GetHours(), CurrentTime.GetMinutes(), CurrentTime.GetSeconds(), GetElapsedDays());
	}
}

int32 ATimeSystem::GetElapsedDays()
{
	return CurrentTime.GetTotalDays();
}

int32 ATimeSystem::CurrentMinute()
{
	return CurrentTime.GetMinutes();
}

int32 ATimeSystem::CurrentHour()
{
	return CurrentTime.GetHours();
}

int32 ATimeSystem::CurrentSecond()
{
	return CurrentTime.GetTotalSeconds();
}
