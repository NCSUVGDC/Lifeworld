// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h" // for EControllerHand
#include "Components/CapsuleComponent.h"
#include "FGesture.h"
#include "../Player/HarlowHand.h"
#include "GestureVolume.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGestureDelegate, AHarlowHand*, Hand);

/**
 * A volume that signals events if a hand is in it and is making the appropraite gesture. It is used
 * to say where and how a player can grip/otherwise interact with an object.
 * 
 * TODO Make this not (just) a capsule component. Spent an hour trying to set up a variable-shape
 * component using the built in classes (e.g. BrushComponent) but couldn't overcome the compilation
 * errors.
 */
UCLASS(ClassGroup=(Custom), Blueprintable, META = (BlueprintSpawnableComponent))
class HARLOWS_WALLPAPER_API UGestureVolume : public UCapsuleComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UGestureVolume();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 
	 * Outputs "<OwningActor>_GestVol_<GestureName>"
	 * The fact that this isn't an override is a bit odd to me. Maybe we should rename it?
	 */
	virtual FString ToString() const;

	/** Shows the volume's capsule boundaries */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		bool DebugDrawCapsule = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug", META=(EditCondition="DebugDrawCapsule"))
		FColor DebugCapsuleColor = FColor(64, 64, 255);

	/**
	 * The gesture that needs to be completed in this volume to signal the event
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
		FGesture Gesture;

	/**
	 * Is the gesture currently being made for either hand?
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
		TMap<EControllerHand, bool> IsGestureMade;

	/** Any hands overlapping this volume (regardless of whether they're making the gesture) */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
		TSet<AHarlowHand*> OverlappingHands;

	/**
	 * Called by AHarlowHand when it detects a change in input. This does not mean that the gesture
	 * changed state from being made -> not being made (or vice versa), rather that it _may_ have
	 * 
	 * @return True if the gesture changed status (being made -> not being made or vice versa)
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		bool NotifyGestureStatus(AHarlowHand* Hand, bool GestureMade);

	/**
	 * @param Self is necessary as a delegate placeholder
	 */
	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap(class UPrimitiveComponent* Self, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
		void OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * Called when a hand overlapping this volume makes the gesture
	 * 
	 * If you want to bind a function in to this (i.e. make your function get called when this one
	 * gets called), in C++ use (in your class) GestureVolume->OnGestureMadeDelegate.AddDynamic(this, &AYourClass:YourFunction),
	 * or in Blueprints use "Bind Event to OnGestureMadeDelegate"
	 */
	UPROPERTY(BlueprintAssignable)
		FOnGestureDelegate OnGestureMadeDelegate;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGestureMade(AHarlowHand* Hand);

	/**
	 * Called when a hand overlapping this volume stops making the gesture
	 * 
	 * If you want to bind a function in to this (i.e. make your function get called when this one
	 * gets called), in C++ use (in your class) GestureVolume->OnGestureStoppedDelegate.AddDynamic(this, &AYourClass:YourFunction),
	 * or in Blueprints use "Bind Event to OnGestureStoppedDelegate"
	 */
	UPROPERTY(BlueprintAssignable)
		FOnGestureDelegate OnGestureStoppedDelegate;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnGestureStopped(AHarlowHand* Hand);

};
