// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorSaturation.h"

#include "Camera/CameraComponent.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AColorSaturation::AColorSaturation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AColorSaturation::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	// Sets lifeworldPlayer to HarlowPawn
	lifeworldPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()))

	// Enable saturation override
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.bOverride_ColorSaturation = true;
	
}

// Called every frame
void AColorSaturation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Set the color saturation of the camera
FVector4 AColorSaturation::SetColorSaturation(FVector4 Saturation)
{
	FVector4 oldSaturation = (Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation;
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation = Saturation;

	return oldSaturation;
}

// Transition from the original saturation to the new saturation
FVector4 AColorSaturation::TransitionColorSaturation(FVector4 Saturation, int seconds)
{
	
}