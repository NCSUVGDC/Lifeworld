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
    playerPawn = Cast<AHarlowPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if (playerPawn == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Cast to HarlowPawn fails"));
        return;
    }
    playerPawn->voicesActivated = true;
    FTimerDelegate voiceTimerDel;
    FString voiceType = "Voice";
	FTimerDelegate whisperTimerDel;
	FString whisperType = "Whisper";
	FTimerDelegate gibberishTimerDel;
	FString gibberishType = "Gibberish";
	FTimerDelegate footstepsTimerDel;
	FString footstepsType = "Footsteps";
    voiceTimerDel.BindUFunction(playerPawn, FName("getVoices"), voiceType);
	whisperTimerDel.BindUFunction(playerPawn, FName("getVoices"), whisperType);
	gibberishTimerDel.BindUFunction(playerPawn, FName("getVoices"), gibberishType);
	footstepsTimerDel.BindUFunction(playerPawn, FName("getVoices"), footstepsType);
    GetWorldTimerManager().SetTimer(whisperTimerHandle, whisperTimerDel, 10.0f, true, 75.0f);
	GetWorldTimerManager().SetTimer(gibberishTimerHandle, gibberishTimerDel, 1.0f, false, 75.0f);
	GetWorldTimerManager().SetTimer(voiceTimerHandle, voiceTimerDel, 5.0f, true, 210.0f);
	GetWorldTimerManager().SetTimer(footstepsTimerHandle, footstepsTimerDel, 1.0f, false, 225.0f);
	GetWorldTimerManager().SetTimer(stopWhisperTimerHandle, this, &AVoices::StopWhisperTimer, 1.0f, false, 210.0f);
}

// Called every frame
void AVoices::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVoices::StopWhisperTimer() 
{
	GetWorld()->GetTimerManager().ClearTimer(whisperTimerHandle);
}

