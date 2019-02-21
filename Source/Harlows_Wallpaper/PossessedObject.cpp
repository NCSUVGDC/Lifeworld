// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessedObject.h"


// Sets default values
APossessedObject::APossessedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APossessedObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APossessedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startLoc != FVector::ZeroVector)
	{
		float num = player->GetDotProductTo(object);
		FString TheFloatStr = FString::SanitizeFloat(num);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);

		if (player->GetDotProductTo(object) >= 0.6)
		{
			object->SetActorLocation(startLoc);
			Destroy();
		}
		else
		{
			object->AddActorLocalOffset(FVector(5, 0, 0));
		}
	}

}

void APossessedObject::setObject(AActor * obj)
{
	object = obj;
	startLoc = obj->GetActorLocation();
}

void APossessedObject::setPlayer(AActor * ply)
{
	player = ply;
}

