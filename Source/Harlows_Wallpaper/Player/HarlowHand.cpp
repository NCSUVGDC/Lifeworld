// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowHand.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
AHarlowHand::AHarlowHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarlowHand::BeginPlay()
{
	Super::BeginPlay();
	
	if (OwningPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AHarlowHand spawned with no owning pawn!"));
	}
	else
	{
		PlayerController = Cast<APlayerController>(OwningPawn->GetController());

		if (PlayerController == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Player pawn missing player controller!"));
		}

		PlayerInput = OwningPawn->InputComponent;

		if (PlayerInput == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Player pawn missing input component!"));
		}
	}

}

const bool AHarlowHand::IsMakingGesture(const FGesture& Gesture)
{
	if (PlayerInput == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't check hand Gesture without InputComponent!"));
		return false;
	}

	// Iterate over all input key-sensitivty pairs; will short circuit and return
	// false if a key is found to be 
	for (auto& KeyValuePair : Gesture.Inputs)
	{
		FName InputName = KeyValuePair.Key;
		FVector2D SensitivityRange = KeyValuePair.Value;

		UE_LOG(LogTemp, Log, TEXT("Getting Input Axis for '%s'..."), *InputName.ToString())

		float CurrentKeyInput = PlayerInput->GetAxisValue(InputName);

		UE_LOG(LogTemp, Log, TEXT("Axis '%s' had value %.2f! Checking if it's within '%s'..."),
			*InputName.ToString(), CurrentKeyInput, *SensitivityRange.ToString());


		// If the key is not within the sensitivity range, immediately return false
		if (SensitivityRange.IsNearlyZero()) // If sensitivity is undefined, any input value above 0 counts
		{
			if (FMath::IsNearlyZero(CurrentKeyInput))
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not satisfy (> 0.0) sensitivity range!"));
				return false;
			}
			
			UE_LOG(LogTemp, Log, TEXT("Did satisfy (> 0.0) sensitivty range!"))
		}
		else // Sensitivity is defined; make sure the current input fits in the range
		{
			if (CurrentKeyInput < SensitivityRange.X)
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not satisfy sensitivity range (too low)!"));
				return false;
			}
			else if (CurrentKeyInput > SensitivityRange.Y)
			{
				UE_LOG(LogTemp, Warning, TEXT("Did not satisfy sensitivity range (too high)!"));
				return false;
			}

			UE_LOG(LogTemp, Log, TEXT("Did satisfy sensitivty range!"))
		}
	} // End looping over all Gesture inputs

	// We didn't return false yet, so all inputs must have been satisfied!
	UE_LOG(LogTemp, Log, TEXT("Player is making Gesture '%s'!"), *Gesture.Name.ToString());
	return true;
}

// Called every frame
void AHarlowHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (PlayerController != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Right Thumbstick X: %.3f ; Right Facebutton 1: %.3f"),
	//		PlayerController->GetInputAnalogKeyState(EKeys::MotionController_Right_Thumbstick_X),
	//		PlayerController->GetInputAnalogKeyState(EKeys::MotionController_Right_FaceButton1)
	//		);
	//}
}

