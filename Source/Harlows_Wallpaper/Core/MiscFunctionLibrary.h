// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// EngineMinimal instead of EngineCore to get the FindObject call in EnumToString to work
#include "EngineMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MiscFunctionLibrary.generated.h"

/** 
 * Converts a boolean value to a C string. Can't be used in UE4's FString::Printf or UE_LOG functions
 * when used with format specifiers - use BoolToTEXT for that
 */
#define BoolToCString(boolval) ((boolval) ? ("true") : ("false"))

/**
 * Converts a boolean value to a UE4 TChar array. Can be used with UE_LOG or FString::Printf
 */
#define BoolToTEXT(boolval) (UTF8_TO_TCHAR(BoolToCString(boolval)))

/**
 * Converts an enum to the UE4 TChar*, needed for string formatting/logging
 */
#define EnumToTEXT(EnumName, EnumVal) (*UMiscFunctionLibrary::EnumToString(EnumName, EnumVal))

/**1
 * Generic useful functions
 */
UCLASS()
class HARLOWS_WALLPAPER_API UMiscFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/**
	 * Converts the enum value to a string. 
	 * E.g. EnumToString("EGestureFinger", EnumVariableSetToIndex) outputs "Index"
	 * 
	 * See also the EnumtoTEXT macro at the top of this file for a shortcut
	 * 
	 * https://answers.unrealengine.com/questions/330496/conversion-of-enum-to-string.html
	 */
	template<typename T>
	static FString EnumToString(const char* EnumName, const T EnumValue)
	{
		UEnum* enumObj = FindObject<UEnum>(ANY_PACKAGE, UTF8_TO_TCHAR(EnumName));
		return *(enumObj ? enumObj->GetNameStringByIndex(static_cast<uint8>(EnumValue)) : "null");
	}

};
