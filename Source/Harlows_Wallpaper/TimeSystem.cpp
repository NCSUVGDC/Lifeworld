// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeSystem.h"


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
	// Create a Timespan based on rate
	Accumulator = FTimespan::FTimespan(0, FGenericPlatformMath::FloorToInt(10 * Rate), 0);
}

// Called every frame
void ATimeSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Update current time
	CurrentTime += Accumulator;
	DebugDisplayTime();
}

void ATimeSystem::DebugDisplayTime() {
	if (GEngine) {
		FString DebugMsg = FString::Printf(TEXT("ElapsedTime: %d:%d:%d"), CurrentTime.GetHours(), CurrentTime.GetMinutes(), CurrentTime.GetSeconds());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}

	UE_LOG(LogTemp, Warning, TEXT("ElapsedTime: %d:%d:%d"), CurrentTime.GetHours(), CurrentTime.GetMinutes(), CurrentTime.GetSeconds());
}
