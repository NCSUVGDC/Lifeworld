// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
#include "HarlowPawn.h"
#include "../Interaction/FGesture.h"
#include "HarlowHand.generated.h"

#define INPUT_AXIS_CHANGED_THRESHOLD 0.001f

UCLASS()
class HARLOWS_WALLPAPER_API AHarlowHand : public AActor
{
	GENERATED_BODY()

private:
	/**
	 * Iterates over all GestureVolumes we're overlapping and checks if we're satisfying any of their
	 * gestures. TODO this could be _way_ more efficient but I've taken long enough to set this system
	 * up so that's a job we'll have to worry about later.
	 */
	void InputAxisChanged(EGestureFinger Finger);

protected:
	virtual void BeginPlay() override;

	/** "Right" or "Left", assigned on BeginPlay based on HandSide */
	FString HandText;

	/** Cached to make accessing the user's input easier */
	APlayerController* PlayerController;

	/** Cached to make accessing the user's input easier [TODO MAY NOT BE NEEDED]*/
	UInputComponent* PlayerInput;

public:	
	AHarlowHand();
	virtual void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent();

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputAxisThumb(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputAxisIndex(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input")
		void InputAxisMiddle(float Val);

	/** 
	 * Useful for both tidy access of any given finger's input value as well as providing a way to 
	 * check if the input has changed since last frame; if it has for any finger, we re-check gestures
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Input")
		TMap<EGestureFinger, float> InputFingerValues;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
		EControllerHand HandSide;

	/** 
	 * The HarlowPawn controlling this Hand
	 * Actual assignment is currently done in Blueprints as that is where hands
	 * are spawned. (If you're curious why, we started off with a copy of the 
	 * UVRF pawn and hands to get the ball rolling faster.)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player", META = (ExposeOnSpawn="True"))
		AHarlowPawn* OwningPawn;

	/** Any gesture volumes that this hand is inside of. Set automatically by the volume when overlapping */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
		TSet<class UGestureVolume*> OverlappedGestureVolumes;

	/** 
	 * Returns whether or not the player is making the given Gesture
	 */
	UFUNCTION(BlueprintCallable)
		const bool IsMakingGesture(UPARAM(ref) const FGesture& Gesture);

	
};
