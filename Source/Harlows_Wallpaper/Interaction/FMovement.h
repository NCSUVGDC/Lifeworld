#pragma once
#include "CoreMinimal.h"
#include "FMovement.generated.h"

/**
 * A "movement" is defined as one or more relative hand transform keyframes relative to an implicit
 * zeroed out keyframe
 * 
 * E.g. Rotating a dial one click has two keyframes: a zeroed out initial transform and a second 
 * transform that's the same location but with a 5 degree rotation on the Z axis,
 *      cranking a level has two keyframes: A zeroed out initial transform, a second transform of 
 * where the hand should be/what angle it should be at with the level pulled down
 * 
 * The initial zero keyframe is implicit and does not need to be included in the list of keyframes.
 * 
 * Movements are combined with poses to create gestures and are a key component of the interactivity
 * framework
 */
USTRUCT(BlueprintType)
struct FMovement
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;

	/**
	 * A sequential list of where/with what rotation the controller must be for it to have done this
	 * movement. These will all be relative to a "hidden"/implicit initial keyframe of zero everything,
	 * which could be in any position/rotation in practice.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FTransform> Keyframes;
};