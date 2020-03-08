// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorSaturation.h"

#include "Camera/CameraComponent.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AColorSaturation::AColorSaturation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Set the transition state to false
	//bInTransition = false;

	//// Set the default value for elapsed and transition time
	//ElapsedTime = 0;
	//TransitionTime = 0;

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

	// Enable saturation override
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.bOverride_ColorSaturation = true;
	
	// Save the current saturation value
	Saturation = (Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation;
	
}

// Called every frame
void AColorSaturation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ElapsedTime += DeltaTime;
	//int alpha = FMath::Clamp<float>(ElapsedTime / TransitionTime, 0, 1);
	//(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation.X =
	//	FMath::Lerp<float, float>(Saturation.X, TargetSaturation.X, alpha);
	//(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation.Y =
	//	FMath::Lerp<float, float>(Saturation.Y, TargetSaturation.Y, alpha);
	//(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation.Z =
	//	FMath::Lerp<float, float>(Saturation.Z, TargetSaturation.Z, alpha);
	//FVector4 result = (Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Transition Saturation is: <%f, %f, %f, %f>"),
	//	TargetSaturation[0], TargetSaturation[1], TargetSaturation[2], TargetSaturation[3]));

	//if (alpha >= 1) {
	//	// Transition complete; reset variables
	//	ElapsedTime = 0;
	//	TransitionTime = 0;
	//	SetActorTickEnabled(false);
	//	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation = TargetSaturation;
	//	FVector4 result = (Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation;
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Transition Complete!"));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Resultant Saturation is: <%f, %f, %f, %f>"),
	//		result[0], result[1], result[2], result[3]));
	//}
}

// Set the color saturation of the camera
FVector4 AColorSaturation::SetColorSaturation(FVector4 NewSaturation)
{
	FVector4 oldSaturation = Saturation;
	Saturation = NewSaturation;
	(Cast<UCameraComponent>(lifeworldCamera))->PostProcessSettings.ColorSaturation = Saturation;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Resultant Saturation is: <%f, %f, %f, %f>"),
		Saturation[0], Saturation[1], Saturation[2], Saturation[3]));
	return oldSaturation;
}

//// Transition from the original saturation to the new saturation
//// Set variables so the Tick() function can do the transition
//FVector4 AColorSaturation::TransitionColorSaturation(FVector4 NewSaturation, float seconds)
//{
//	FVector4 oldSaturation = Saturation;
//	FVector4 TargetSaturation = NewSaturation;
//	TransitionTime = FMath::Clamp<float>(seconds, 0.001, FLT_MAX);
//	ElapsedTime = 0;
//	SetActorTickEnabled(true);
//	return oldSaturation;
//}