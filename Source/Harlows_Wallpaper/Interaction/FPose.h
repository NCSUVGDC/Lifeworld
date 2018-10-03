#pragma once
#include "CoreMinimal.h"
#include "FPose.generated.h"

/**
 * A "pose" is defined as one or more buttons held at a defined sensitivity. 
 * 
 * E.g. A pinch is defined as holding the trigger and one or more face buttons,
 *      a grip is defined as holding the trigger, grip, and a face button
 * 
 * Poses are combined with movements to create gestures and are a key component of the interactivity
 * framework
 */
USTRUCT(BlueprintType)
struct FPose
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;

	/**
	 * Names of each input axis/action that are each needed for this pose, as well as the min/max 
	 * sensitivity thresholds for each (only applicable to axes)
	 * 
	 * Note to self: See UPlayerInput.AxisKeyMap and ActionKeyMap for map of input names.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FName, FVector2D> Inputs;


	FORCEINLINE bool operator==(const FPose& Other) const
	{
		return this->Name == Other.Name;
	}
};

/** For use with TMaps */
FORCEINLINE uint32 GetTypeHash(const FPose& Pose)
{
	
	return GetTypeHash(Pose.Name);
}