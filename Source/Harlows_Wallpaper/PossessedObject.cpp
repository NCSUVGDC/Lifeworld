// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessedObject.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

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

	float num = player->GetDotProductTo(object);
	//float num = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).X);
	FString TheFloatStr = FString::SanitizeFloat(num);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);

	if (player->GetDotProductTo(object) >= .83 && !isSpotted )
	{
		isSpotted = true;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));

		ddX = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).X);
		ddY = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).Y);
	}
	else if ( isSpotted )
	{
		FString ddxStr = "ddX: " + FString::SanitizeFloat(ddX);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *ddxStr);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *ddxStr);
		if (ddX > 1)
		{
			ddX -= 1;
			object->AddActorWorldOffset(FVector(-1, 0, 0));
		}

		if (ddY > 1)
		{
			ddY -= 1;
			object->AddActorWorldOffset(FVector(0, -1, 0));
		}

		if (ddX < 1 && ddY < 1)
		{
			Destroy();
			isSpotted = false;
			object->SetActorLocation(startLoc);
		}
	}
	else if( iterationCount != 100 )
	{
		object->AddActorWorldOffset(FVector(0, 0.2, 0));
		iterationCount++;
	}
/*
	if (isSpotted)
	{


	}*/
}

void APossessedObject::setObject(AActor * obj)
{
	object = obj;
	startLoc = obj->GetActorLocation();
	startRot = obj->GetActorRotation();
}

void APossessedObject::setPlayer(AActor * ply)
{
	player = ply;
}

void APossessedObject::DoASpoopyThing()
{



}

