// Fill out your copyright notice in the Description page of Project Settings.

#include "BreathingWalls.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BrushComponent.h"


// Sets default values
ABreathingWalls::ABreathingWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABreathingWalls::BeginPlay()
{
	Super::BeginPlay();
	FTimerDelegate voiceTimerDel;
	FString voiceType = "Voice";
	FTimerDelegate whisperTimerDel;
	FString whisperType = "Whisper";
	FTimerDelegate gibberishTimerDel;
	FString gibberishType = "Gibberish";
	FTimerDelegate footstepsTimerDel;
	FString footstepsType = "Footsteps";
	GetWorldTimerManager().SetTimer(whisperTimerHandle, whisperTimerDel, 30.0f, true, 75.0f);
	GetWorldTimerManager().SetTimer(gibberishTimerHandle, gibberishTimerDel, 1.0f, false, 75.0f);
	GetWorldTimerManager().SetTimer(voiceTimerHandle, voiceTimerDel, 5.0f, true, 210.0f);
	GetWorldTimerManager().SetTimer(footstepsTimerHandle, footstepsTimerDel, 1.0f, false, 225.0f);
	
}

// Called every frame
void ABreathingWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

