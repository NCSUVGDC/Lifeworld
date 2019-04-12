// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessedObject.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
APossessedObject::APossessedObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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
}

//Set refence to object that will be moved as part of this symptom
void APossessedObject::setObject(AActor * obj)
{
	object = obj;
	startLoc = obj->GetActorLocation();
	startRot = obj->GetActorRotation();
	symptomTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

//Set refence to the player
void APossessedObject::setPlayer(AActor * ply)
{
	player = ply;
}

//Sp00k
void APossessedObject::DoASpoopyThing()
{

}

//Update per frame, called by SymptomManager
void APossessedObject::Update()
{
	//If object has not yet been spotted...
	if ( !isSpotted)
	{
		//Check if the object being moved is in direct view of the player
		if (player->GetDotProductTo(object) >= .83)
		{
			isSpotted = true;
			//Store X & Y distance of object from its starting location
			ddX = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).X);
			ddY = UKismetMathLibrary::Abs((object->GetActorLocation() - startLoc).Y);
		}
		//otherwise, if the object hasn't moved too far away yet, move a lil bit
		else if (iterationCount != 100)
		{
			object->AddActorWorldOffset(FVector(0, 0.2, 0));
			iterationCount++;
		}
	}
	//otherwise...
	else 
	{
		//If not close to original location in X direction, move closer
		if (ddX > 1)
		{
			ddX -= 1;
			object->AddActorWorldOffset(FVector(-1, 0, 0));
		}
		//If not close to original location in Y direction, move closer
		if (ddY > 1)
		{
			ddY -= 1;
			object->AddActorWorldOffset(FVector(0, -1, 0));
		}
		//If object is close enough to original location, snap back into place and destroy this possessed object
		if (ddX < 1 && ddY < 1)
		{
			object->SetActorLocation(startLoc);
			Destroy();
		}
	}

	//If it has been 19 seconds since symptom began, it is likely that the symptom manager is close to done updating this
    //In which case, play it safe and go ahead and retire the symptom and return object to its origin
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) - symptomTime > 19.0f)
	{
		object->SetActorLocation(startLoc);
		Destroy();
	}
}

