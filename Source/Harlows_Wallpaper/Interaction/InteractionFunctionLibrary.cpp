// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionFunctionLibrary.h"


void UInteractionFunctionLibrary::TriggerNarrativeEvent(FNarrativeEvent& Event)
{
	Event.TestDelegate.Execute();
}