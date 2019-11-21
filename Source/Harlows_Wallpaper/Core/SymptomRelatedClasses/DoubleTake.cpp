// Fill out your copyright notice in the Description page of Project Settings.

#include "DoubleTake.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"


// Sets default values
ADoubleTake::ADoubleTake()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADoubleTake::BeginPlay()
{
	Super::BeginPlay();

	// Get list of all Actors in the game
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// If the actor has the "possess" tag, add it to the list of possibly manipulated actors
	for (AActor* Actor : FoundActors)
	{
		// Check for tags
		if (Actor->Tags.Num() != 0)
		{
			// Loop backwards over the list so we can safely remove tags as we iterate
			for (int Idx = Actor->Tags.Num() - 1; Idx >= 0; Idx--)
			{
				FName& TagName = Actor->Tags[Idx];

				//check if the tag says it can be moved by DoubleTakeSymptom
				if (TagName == FName("possess"))
				{
					DoubleTakeActors.Add(Actor);
				}
			}
		}
	}

	setPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
}

void ADoubleTake::setPlayer(AActor * ply)
{
	player = ply;
}

// Called every frame
void ADoubleTake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoubleTake::setObject(AActor * obj)
{
	iterationCount = 0;
	object = obj;
	startLoc = obj->GetActorLocation();
	startRot = obj->GetActorRotation();
}


void ADoubleTake::SelectRandomUnseenObject()
{
	AActor* chosenObject = ASymptomHelper::SelectRandomUnseen(DoubleTakeActors);

	if (chosenObject == NULL)
	{
		FTimerHandle timerHandle;

		GetWorldTimerManager().SetTimer(timerHandle, this, &ADoubleTake::SelectRandomUnseenObject, 3.0f, false);
	}
	else
	{
		setObject(chosenObject);
	}
}

void ADoubleTake::Update()
{
	//If the object has not yet been spotted...
	if (!isSpotted  && isRunning)
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
		else if (iterationCount != 40)
		{
			object->AddActorWorldOffset(FVector(0, 0.2, 0));
			iterationCount++;
		}
	}
	//otherwise...
	else if (isRunning)
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
			EndSymptom();
		}
	}
}

void ADoubleTake::EndSymptom()
{
	iterationCount = 0;
	isSpotted = false;
	isRunning = false;
	object->SetActorLocation(startLoc);
	object->SetActorRotation(startRot);
}

TArray<AActor*> ADoubleTake::getDoubleTakeActors() const
{
	return DoubleTakeActors;
}

/**
//If we dont find a StaticMeshActor within player's periphery, this will allow us to at least possess the closest thing
	AActor* closestObject = DoubleTakeActors[0];
	float closestDot = -1;

	// Iterate through list of possessable objects
	for (AActor* object : DoubleTakeActors)
	{
		float dotProd = (GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager)->GetDotProductTo(object);
		//If object is in periphery, possess with this symptom
		if (dotProd >= 0.7 && dotProd < 0.8)
		{
			setObject(object);
			return true;
		}
		//If not in periphery, check if it is closer to player's view than current "closestObject"
		else if (dotProd > closestDot && dotProd < .8)
		{
			closestObject = object;
			closestDot = dotProd;
		}
	}

	//If a good candidate was not found, use the closest one
	setObject(closestObject);
	return true;


**/