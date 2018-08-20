// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FNarrativeEvent.h"
#include "InteractionFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HARLOWS_WALLPAPER_API UInteractionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
	UFUNCTION(BlueprintCallable)
		static void TriggerNarrativeEvent(UPARAM(ref) FNarrativeEvent& Event);
	
};
