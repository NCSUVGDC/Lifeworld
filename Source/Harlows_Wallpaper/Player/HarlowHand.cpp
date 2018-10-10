// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowHand.h"
#include "../Interaction/GestureVolume.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

// Sets default values
AHarlowHand::AHarlowHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the input fingers map
	InputFingerValues.Empty();
	InputFingerValues.Add(EGestureFinger::Thumb);
	InputFingerValues.Add(EGestureFinger::Index);
	InputFingerValues.Add(EGestureFinger::Middle);
}

// Called when the game starts or when spawned
void AHarlowHand::BeginPlay()
{
	Super::BeginPlay();

	HandText = (HandSide == EControllerHand::Right ? "Right" : "Left");
	
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
		else
		{
			SetupPlayerInputComponent();
		}
	}

}

void AHarlowHand::SetupPlayerInputComponent()
{
	// Note: Does not properly account for EControllerHand values that aren't left or right

	FString Fuck = FString::Printf(TEXT("%s Thumb"), *HandText);
	FName Shit = FName(*Fuck);

	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Thumb"), *HandText)), this, &AHarlowHand::InputAxisThumb);
	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Index"), *HandText)), this, &AHarlowHand::InputAxisIndex);
	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Middle"), *HandText)), this, &AHarlowHand::InputAxisMiddle);

}

void AHarlowHand::InputAxisThumb(float Val)
{
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Thumb], INPUT_AXIS_CHANGED_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Thumb] = Val;
		InputAxisChanged(EGestureFinger::Thumb);
	}
}

void AHarlowHand::InputAxisIndex(float Val)
{
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Index], INPUT_AXIS_CHANGED_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Index] = Val;
		InputAxisChanged(EGestureFinger::Index);
	}
}

void AHarlowHand::InputAxisMiddle(float Val)
{
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Middle], INPUT_AXIS_CHANGED_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Middle] = Val;
		InputAxisChanged(EGestureFinger::Middle);
	}
}

void AHarlowHand::InputAxisChanged(EGestureFinger Finger)
{
	for (UGestureVolume* GestureVolume : OverlappedGestureVolumes)
	{
		bool MakingGesture = IsMakingGesture(GestureVolume->Gesture);

		if (MakingGesture != GestureVolume->IsGestureMade)
		{
			GestureVolume->IsGestureMade = MakingGesture;

			if (MakingGesture == true)
			{
				GestureVolume->OnGestureMadeDelegate.Broadcast(this);
			}
			else
			{
				GestureVolume->OnGestureStoppedDelegate.Broadcast(this);
			}
		}
	}
}


const bool AHarlowHand::IsMakingGesture(const FGesture& Gesture)
{
	// Iterate over all input key-sensitivty pairs; will short circuit and return
	// false if a key is found to be 
	for (auto& KeyValuePair : Gesture.Inputs)
	{
		EGestureFinger Finger = KeyValuePair.Key;
		FVector2D SensitivityRange = KeyValuePair.Value;

		float CurrentFingerInput = InputFingerValues[Finger];

		if (CurrentFingerInput < SensitivityRange.X)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Did not satisfy sensitivity range (too low)!"));
			return false;
		}
		else if (CurrentFingerInput > SensitivityRange.Y)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Did not satisfy sensitivity range (too high)!"));
			return false;
		}

		//UE_LOG(LogTemp, Log, TEXT("Did satisfy sensitivty range!"))
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

