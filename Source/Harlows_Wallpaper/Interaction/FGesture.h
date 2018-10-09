#pragma once
#include "CoreMinimal.h"
#include "FGesture.generated.h"

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
	 * Names of each input axis/action that are each needed for this gesture, as well as the min/max 
	 * sensitivity thresholds for each. A sensitivity threshold of (0, 1) means the input is optional.
	 * 
	 * Note: See UPlayerInput.AxisKeyMap and ActionKeyMap for map of input names
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TMap<FName, FVector2D> Inputs;

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