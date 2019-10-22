// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowHand.h"
#include "../Interaction/GestureVolume.h"
#include "../Harlows_Wallpaper.h"
#include "../Core/MiscFunctionLibrary.h"
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

	HandSideText = (HandSide == EControllerHand::Right ? "Right" : "Left");

	// Add our hand side to tha actor's name
	FString newName = FString::Printf(TEXT("HarlowHand_%s"), *HandSideText);
	Rename(*newName);
	//this->SetActorLabel(*newName, true);

	if (OwningPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("'*s' spawned with no owning pawn!"), *GetName());
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
	UE_LOG(LogTemp, Log, TEXT("Setting up input for hand '%s'"), *HandSideText);
	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Thumb"), *HandSideText)), this, &AHarlowHand::InputAxisThumb);
	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Index"), *HandSideText)), this, &AHarlowHand::InputAxisIndex);
	PlayerInput->BindAxis(FName(*FString::Printf(TEXT("%s Middle"), *HandSideText)), this, &AHarlowHand::InputAxisMiddle);

}

void AHarlowHand::InputAxisThumb(float Val)
{
	//UE_LOG(LogTemp, Log, TEXT("%s Thumb: %.3f"), *HandSideText, Val);
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Thumb], INPUT_AXIS_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Thumb] = Val;
		InputAxisChanged(EGestureFinger::Thumb);
	}
}

void AHarlowHand::InputAxisIndex(float Val)
{
	//UE_LOG(LogTemp, Log, TEXT("%s Index: %.3f"), *HandSideText, Val);
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Index], INPUT_AXIS_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Index] = Val;
		InputAxisChanged(EGestureFinger::Index);
	}
}

void AHarlowHand::InputAxisMiddle(float Val)
{
	//UE_LOG(LogTemp, Log, TEXT("%s Middle: %.3f"), *HandSideText, Val);
	// Detect if the input has changed (since this method is called every frame)
	if (FMath::IsNearlyEqual(Val, InputFingerValues[EGestureFinger::Middle], INPUT_AXIS_THRESHOLD) == false)
	{
		InputFingerValues[EGestureFinger::Middle] = Val;
		InputAxisChanged(EGestureFinger::Middle);
	}
}

void AHarlowHand::InputAxisChanged(EGestureFinger Finger)
{
	UE_LOG(Interaction, Log, TEXT("Finger Input Changed: '%s %s' (now %.2f)"), *HandSideText,
		*UMiscFunctionLibrary::EnumToString("EGestureFinger", Finger), InputFingerValues[Finger]);
	for (UGestureVolume* GestureVolume : OverlappedGestureVolumes)
	{
		bool MakingGesture = IsMakingGesture(GestureVolume->Gesture);
		GestureVolume->NotifyGestureStatus(this, MakingGesture);
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

		// TODO it would make more sense if this comparison logic was in FGesture's code...
		// CurrentFingerInput <= SensitivityRange.X, accounting for floating point inaccuracies (actually gave issues this time when the lower bound was 0.0)
		if (CurrentFingerInput < SensitivityRange.X || FMath::IsNearlyEqual(CurrentFingerInput, SensitivityRange.X, INPUT_AXIS_THRESHOLD))
		{
			//UE_LOG(Interaction, Warning, TEXT("Did not satisfy sensitivity range (too low)!"));
			return false;
		}
		else if (CurrentFingerInput > SensitivityRange.Y)
		{
			//UE_LOG(Interaction, Warning, TEXT("Did not satisfy sensitivity range (too high)!"));
			return false;
		}
		else
		{
			UE_LOG(Interaction, Log, TEXT("Detecting finger '%s' input value '%f' to be within limits '%s'"), 
				EnumToTEXT("EGestureFinger", Finger), CurrentFingerInput, *SensitivityRange.ToString());
		}

		//UE_LOG(LogTemp, Log, TEXT("Did satisfy sensitivty range!"))
	} // End looping over all Gesture inputs

	// We didn't return false yet, so all inputs must have been satisfied!
	UE_LOG(Interaction, Log, TEXT("Player is making Gesture '%s'!"), *Gesture.Name.ToString());
	return true;
}

// Called every frame
void AHarlowHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Sample code using input-keys instead of project-based named inputs
	//if (PlayerController != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Right Thumbstick X: %.3f ; Right Facebutton 1: %.3f"),
	//		PlayerController->GetInputAnalogKeyState(EKeys::MotionController_Right_Thumbstick_X),
	//		PlayerController->GetInputAnalogKeyState(EKeys::MotionController_Right_FaceButton1)
	//		);
	//}
}

