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
	FString TheFloatStr = FString::SanitizeFloat(num);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);

	if (player->GetDotProductTo(object) >= .75)
	{
		isSpotted = true;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));

			float ddX = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).X);
			float ddY = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).Y);
	//	object->SetActorLocation(startLoc);
	//	object->SetActorRotation(startRot);
	//	Destroy();
	}
	else
	{
		object->AddActorWorldOffset(FVector(0.05, 0, 0));
	//	object->AddActorWorldRotation(FRotator(0,0,0.05));
	}

	if (isSpotted)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Retreat!"));
		if ( ddX > .5 )
		{
			ddX -= .1;
			object->AddActorWorldOffset(FVector(-.1, 0, 0));
		}
		if (ddY > .5)
		{
			ddY -= .1;
			object->AddActorWorldOffset(FVector(0, -.1, 0));
		}
		else if (ddX < .5 && ddY < .5)
		{
			Destroy();
			object->SetActorLocation(startLoc);
		}

	}
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

