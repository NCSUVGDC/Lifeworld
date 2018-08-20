#pragma once
#include "CoreMinimal.h"
#include "FNarrativeEvent.generated.h"


DECLARE_DYNAMIC_DELEGATE( FNarrativeEventDelegate );

USTRUCT(BlueprintType)
struct FNarrativeEvent
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite)
		FName Name;

	UPROPERTY(BlueprintReadWrite)
		FNarrativeEventDelegate TestDelegate;
};