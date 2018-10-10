// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CustomGameSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Custom Settings"))
class HARLOWS_WALLPAPER_API UCustomGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	/** Toggles in-game debug messages for marked gameplay functions */
	UPROPERTY(EditAnywhere, config, Category = InGame)
	bool bEnableDebugMessages;
};
