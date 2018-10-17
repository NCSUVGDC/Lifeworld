// Fill out your copyright notice in the Description page of Project Settings.

#include "GestureVolume.h"
#include "../Core/MiscFunctionLibrary.h"
#include "../Harlows_Wallpaper.h"


UGestureVolume::UGestureVolume()
{
	OnComponentBeginOverlap.AddDynamic(this, &UGestureVolume::OnBeginOverlap);
	OnGestureMadeDelegate.AddDynamic(this, &UGestureVolume::OnGestureMade);
	OnGestureStoppedDelegate.AddDynamic(this, &UGestureVolume::OnGestureStopped);

	// Initialize IsGestureMade
	// Note: Any EControllerHand value not defined here will cause an assert if we try to access it
	IsGestureMade.Empty();
	IsGestureMade.Add(EControllerHand::Left, false);
	IsGestureMade.Add(EControllerHand::Right, false);
}

void UGestureVolume::BeginPlay()
{
	Super::BeginPlay();
}

FString UGestureVolume::ToString() const
{
	return FString::Printf(TEXT("%s_GestVol_%s"), 
		GetOwner() == nullptr ? TEXT("null") : *(GetOwner()->GetName()),
		*Gesture.Name.ToString());
}

bool UGestureVolume::NotifyGestureStatus(AHarlowHand* Hand, bool GestureMade)
{
	// Check that the gesture changed status
	// Using Find() with a bool pointer here as trying to access the value of an undefined key will
	// cause an assert
	bool* CurrentGestureStatusPtr = IsGestureMade.Find(Hand->HandSide);

	if(CurrentGestureStatusPtr == nullptr) // null means the HandSide didn't exist in our map
	{
		UE_LOG(Interaction, Error, TEXT("Hand side '%s' undefined for Gesture Volume '%s'"), 
			*UMiscFunctionLibrary::EnumToString("EControllerHand", Hand->HandSide), *ToString());
	}

	// Return early if the gesture didn't change status
	if (*CurrentGestureStatusPtr == GestureMade)
	{
		return false;
	}

	// Gesture must have changed status!
	IsGestureMade[Hand->HandSide] = GestureMade;

	UE_LOG(Interaction, Log, TEXT("Gesture '%s' on volume '%s' changed to '%s' for hand '%s'"),
		*Gesture.Name.ToString(), *ToString(), BoolToTEXT(GestureMade), *UMiscFunctionLibrary::EnumToString("EControllerHand", Hand->HandSide) );

	if (GestureMade == true)
	{
		OnGestureMadeDelegate.Broadcast(Hand);
	}
	else
	{
		OnGestureStoppedDelegate.Broadcast(Hand);
	}

	return true; // We know the gesture changed if we've made it this far
}

void UGestureVolume::OnBeginOverlap(class UPrimitiveComponent* Self, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( AHarlowHand* OverlappingHand = Cast<AHarlowHand>(OtherActor) )
	{
		// Overlapping actor is a hand
		UE_LOG(Interaction, Log, TEXT("Overlapped by hand '%s'!"), *OverlappingHand->GetName());

		OverlappingHand->OverlappedGestureVolumes.Add(this);
		OverlappingHands.Add(OverlappingHand);
	}
	else
	{
		UE_LOG(Interaction, Log, TEXT("Overlapped by non-hand actor '%s'!"), OtherActor != nullptr ? *OtherActor->GetName() : TEXT("NULL") );
	}
}

void UGestureVolume::OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ( AHarlowHand* OverlappingHand = Cast<AHarlowHand>(OtherActor) )
	{
		UE_LOG(Interaction, Log, TEXT("No longer overlapped by hand '%s'!"), *OverlappingHand->GetName());

		OverlappingHand->OverlappedGestureVolumes.Remove(this);
		OverlappingHands.Remove(OverlappingHand);
	}
}

/** OnGestureMade is a BlueprintNativeEvent, so we must add "_Implementation" to it's C++ function definition */
void UGestureVolume::OnGestureMade_Implementation(AHarlowHand* Hand)
{
	UE_LOG(Interaction, Log, TEXT("Hand '%s' made gesture '%s' in GestureVolume '%s'"), 
		*Hand->GetName(), *Gesture.Name.ToString(), *this->GetName());
}


/** OnGestureStopped is a BlueprintNativeEvent, so we must add "_Implementation" to it's C++ function definition */
void UGestureVolume::OnGestureStopped_Implementation(AHarlowHand* Hand)
{
	UE_LOG(Interaction, Log, TEXT("Hand '%s' stopped making gesture '%s' in GestureVolume '%s'"), 
		*Hand->GetName(), *Gesture.Name.ToString(), *this->GetName());
}