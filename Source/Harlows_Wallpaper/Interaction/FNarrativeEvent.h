#pragma once
#include "CoreMinimal.h"
#include "FNarrativeEvent.generated.h"


DECLARE_DYNAMIC_DELEGATE( FNarrativeEventDelegate );

USTRUCT(BlueprintType)
struct FNarrativeEvent
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FNarrativeEventDelegate TestDelegate;
};