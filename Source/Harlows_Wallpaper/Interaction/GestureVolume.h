// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "FGesture.h"
#include "FNarrativeEvent.h"
#include "GestureVolume.generated.h"

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
	
	
public:

	UGestureVolume();

	/**
	 * Gesture that needs to be completed in this volume to signal the event
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Harlow")
		FGesture Gesture;

	/**
	 * Event to signal once this gesture has been completed
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Harlow")
		FNarrativeEvent Event;

	/**
	 * @param Self is necessary as a delegate placeholder
	 */
	UFUNCTION(BlueprintCallable)
		void OnBeginOverlap(class UPrimitiveComponent* Self, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// UFUNCTION(BlueprintCallable)
	// 	void OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
