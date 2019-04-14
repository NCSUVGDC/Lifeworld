// Fill out your copyright notice in the Description page of Project Settings.

#include "Voices.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
AVoices::AVoices()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVoices::BeginPlay()
{
	Super::BeginPlay();
	/*
    playerPawn = Cast<AHarlowPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerPawn == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Cast to HarlowPawn fails"));
        return;
    }
    playerPawn->voicesActivated = true;
    FTimerDelegate voiceTimerDel;
    FString voiceType = "General";
    voiceTimerDel.BindUFunction(this, FName("getVoices"), voiceType);
    GetWorldTimerManager().SetTimer(voiceTimerHandle, voiceTimerDel, 10.0f, true, 0.0f);
	*/
}

// Called every frame
void AVoices::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
    FTimespan timePassed = FTimespan::FromSeconds(DeltaTime);
    timeSinceLastFire += timePassed;
	*/
}

