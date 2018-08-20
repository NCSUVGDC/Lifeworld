// Fill out your copyright notice in the Description page of Project Settings.

#include "GestureVolume.h"


UGestureVolume::UGestureVolume()
{
	OnComponentBeginOverlap.AddDynamic(this, &UGestureVolume::OnBeginOverlap);
}

void UGestureVolume::OnBeginOverlap(class UPrimitiveComponent* DJObviouslyDoesntKnowWhatHesDoing, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlap!"));
}