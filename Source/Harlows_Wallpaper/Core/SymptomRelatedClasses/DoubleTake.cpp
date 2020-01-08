#include "DoubleTake.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"

//Sets default values
ADoubleTake::ADoubleTake()
{
	//Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

TArray<AActor*> ADoubleTake::getDoubleTakeActors() const
{
	return DoubleTakeActors;
}

//Called when the game starts or when spawned
void ADoubleTake::BeginPlay()
{
	Super::BeginPlay();

	//Get list of all Actors in the game
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	//If the actor has the proper tag, add it to the list of possibly manipulated actors
	for (AActor* Actor : FoundActors)
	{
		//Check if the actor has any tags
		if (Actor->Tags.Num() != 0)
		{
			//Loop backwards over the list of tags
			for (int Idx = Actor->Tags.Num() - 1; Idx >= 0; Idx--)
			{
				FName& TagName = Actor->Tags[Idx];

				//If the tag says it can be moved by this symptom, add it to the list
				if (TagName == FName(*DoubleTakeTag))
				{
					DoubleTakeActors.Add(Actor);
				}
			}
		}
	}

	//tell the manager what the player actor is
	SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
}

//tell the manager which actor is the player
void ADoubleTake::SetPlayer(AActor * ply)
{
	_Player = ply;
}

/** Select a random unseen object for the double take symptom.
 * @Return: true if a proper object is found, false if not
 */
bool ADoubleTake::StartSymptom(float inPeripheralViewBound)
{
	//find a random unseen object with the tag "DoubleTake"
	AActor* chosenObject = ASymptomHelper::SelectRandomUnseen(DoubleTakeActors);

	if (chosenObject != NULL)
	{
		_Object = chosenObject;

		_StartLoc = _Object->GetActorLocation();
		_StartRot = _Object->GetActorRotation();

		_IsSpotted = false;

		_PeripheralViewBound = inPeripheralViewBound;

		SetActorTickEnabled(true);

		return true;
	}

	return false;
}

/** Reset the object's position and orientation and end the symptom */
void ADoubleTake::EndSymptom()
{
	_Object->SetActorLocation(_StartLoc);
	_Object->SetActorRotation(_StartRot);

	SetActorTickEnabled(false);
}

/** Find a direction the object can move in that won't take it into another object or make it float above ground */
void ADoubleTake::ChooseTargetLocation(float inMaxDistanceAllowed)
{

}

//Called every frame
void ADoubleTake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_IsSpotted == false)
	{
		//Check if the object being moved is within view of the player
		if (_Player->GetDotProductTo(_Object) >= _PeripheralViewBound)
		{
			_IsSpotted = true;

			//Store X & Y distance of object from its starting location
			ddX = UKismetMathLibrary::Abs((_Object->GetActorLocation() - _StartLoc).X);
			ddY = UKismetMathLibrary::Abs((_Object->GetActorLocation() - _StartLoc).Y);
		}
	}
}

/**
void ADoubleTake::Update()
{
	//If the object has not yet been spotted...
	if (!isSpotted  && isRunning)
	{
		
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
}*/


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