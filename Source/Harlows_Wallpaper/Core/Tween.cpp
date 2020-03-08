// Fill out your copyright notice in the Description page of Project Settings.

#include "Tween.h"
#include "Tickable.h"

/** Create a new tween that will iterate over a time period and return a value based on time between a start and end value */
Tween::Tween(float inStartValue, float inEndValue, float inTimePeriod)
{
	_StartValue = inStartValue;
	_EndValue = inEndValue;
	_TimePeriod = inTimePeriod;

	//get time system reference

}

//Called every frame
void Tween::Tick(float inDeltaTime)
{
	_TimeElapsed += inDeltaTime;
}

/** Return the value in the interval at this time */
float Tween::GetValue()
{
	float t = _TimeElapsed / _TimePeriod;
	return _StartValue + t * (_EndValue - _StartValue);
}
