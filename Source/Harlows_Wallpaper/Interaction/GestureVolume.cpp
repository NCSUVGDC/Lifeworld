// Fill out your copyright notice in the Description page of Project Settings.

#include "GestureVolume.h"
#include "../Harlows_Wallpaper.h"
#include "../Player/HarlowHand.h"

UGestureVolume::UGestureVolume()
{
	OnComponentBeginOverlap.AddDynamic(this, &UGestureVolume::OnBeginOverlap);
}

void UGestureVolume::OnBeginOverlap(class UPrimitiveComponent* Self, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( AHarlowHand* OverlappingHand = Cast<AHarlowHand>(OtherActor) )
	{
		// Overlapping actor is a hand 
		UE_LOG(Interaction, Log, TEXT("Overlapped by hand '%s'!"), *OverlappingHand->GetName());

		// Make sure hand is forming poses
		bool MakingAllPoses = false;
		for (auto& KeyValuePair : Gesture.Sequence)
		{
			FPose& Pose = KeyValuePair.Key;

			MakingAllPoses &= OverlappingHand->IsMakingPose(Pose);
		}
		UE_LOG(LogTemp, Log, TEXT("Making All Poses for GestureVolume: %d"), MakingAllPoses);
	}
	else
	{
		UE_LOG(Interaction, Log, TEXT("Overlapped by non-hand actor '%s'!"), OtherActor != nullptr ? *OtherActor->GetName() : TEXT("NULL") );
	}
}