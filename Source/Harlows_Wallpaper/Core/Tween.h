// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** Class that tracks interval between two values over a given time period
 * 
 */
class HARLOWS_WALLPAPER_API Tween
{
	/** Starting value of the interval */
	float _StartValue = 0.0f;

	/** Ending value of the interval */
	float _EndValue = 1.0f;

	/** Time taken for the tween to cross from the start value to the end value */
	float _TimePeriod = 0.1f;

	/** time since creation of the Tween */
	float _TimeElapsed = 0.0f;

public:
	Tween(float inStartValue, float inEndValue, float inTimePeriod);

	/** Return the interval value for this time within the _TimePeriod */
	float GetValue();

	void Tick(float inDeltaTime);


};
