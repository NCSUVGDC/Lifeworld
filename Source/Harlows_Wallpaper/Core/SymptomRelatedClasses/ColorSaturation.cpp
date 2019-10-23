// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorSaturation.h"

#include "Camera/CameraComponent.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AColorSaturation::AColorSaturation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AColorSaturation::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	// Sets lifeworldPlayer to HarlowPawn
	lifeworldPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Sets lifeworldCamera to the camera component of lifeworldPlayer
	lifeworldCamera = Cast<USceneComponent>(lifeworldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));
	
}

// Called every frame
void AColorSaturation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Set the color saturation of the camera
FVector4 AColorSaturation::SetColorSaturation(FVector4 Saturation)
{
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.bOverride_ColorSaturation = true;
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation = Saturation;

	return Saturation;
}
