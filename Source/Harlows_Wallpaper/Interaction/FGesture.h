#pragma once
#include "CoreMinimal.h"
#include "FGesture.generated.h"

/** 
 * Instead of viewing the controller as a bunch of buttons we interpret it as one of these three 
 * fingers, with the thumb being mapped to the joystick/any face button and the index and middle 
 * fingers being mapped to their respective triggers
 */
UENUM(BlueprintType)
enum class EGestureFinger : uint8
{
	Thumb,
	Index,
	Middle
};

/**
 * A "gesture" is defined as one or more buttons held at a defined sensitivity. 
 * 
 * E.g. A pinch is defined as holding the trigger and one or more face buttons,
 *      a grip is defined as holding the trigger, grip, and a face button,
 *      a poke is defined as holding the grip (and optionally a face button)
 * 
 */
USTRUCT(BlueprintType)
struct FGesture
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;

	/**
	 * Each finger needed for this gesture, as well as the min/max sensitivity thresholds for each.
	 * A sensitivity threshold of (0, 1) implies the input is optional.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<EGestureFinger, FVector2D> Inputs;

	FORCEINLINE bool operator==(const FGesture& Other) const
	{
		return this->Name == Other.Name;
	}
};

/** For use with TMaps */
FORCEINLINE uint32 GetTypeHash(const FGesture& Gesture)
{
	return GetTypeHash(Gesture.Name);
}