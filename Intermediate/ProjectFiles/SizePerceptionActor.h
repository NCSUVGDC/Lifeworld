#pragma once

#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"

class SizePerceptionActor
{
public:
	SizePerceptionActor();
	~SizePerceptionActor();
	AStaticMeshActor Select();

protected:
	// Called when the game starts or when spawned
	void BeginPlay();

private:
	// An array of actors that can be manipulated with size perception
	UPROPERTY()
		TArray<AActor*> SizePerceptionActors;

};

