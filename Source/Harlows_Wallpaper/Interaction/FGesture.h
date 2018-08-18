#pragma once
#include "CoreMinimal.h"
#include "FPose.h"
#include "FMovement.h"
#include "FGesture.generated.h"

/**
 * A "gesture" is defined as a sequence of poses and movements. When the gesture is complete, one or
 * more events can be signaled
 * 
 * E.g. Pulling a lamp chain: Pinch (pose) on the chain, pull (movement) down while still pinching.
 * Gesture is complete so the lamp would then turn on or off.
 *      Unlocking a combination lock: Pinch (pose) on the dial, rotate (movement) a few clicks, 
 * rotate (movement) a few clicks to the next number, etc. The correct numbers are defined by 
 * specifying the rotation amounts
 */
USTRUCT(BlueprintType)
struct FGesture
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite)
		FName Name;

	/**
	 * List of each pose and it's movements that must be completed in order to signal the target event
	 */
	UPROPERTY(BlueprintReadWrite)
		TMap<FPose, FMovement> Sequence;

};