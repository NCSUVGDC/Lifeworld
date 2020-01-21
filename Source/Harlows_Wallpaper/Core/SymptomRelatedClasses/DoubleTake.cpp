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

	SetActorTickEnabled(false);

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
bool ADoubleTake::StartSymptom(float inMaxDistanceFromOriginAllowed, float inPeripheralViewBound)
{
	//find a random unseen object with the tag "DoubleTake"
	AActor* chosenObject = ASymptomHelper::SelectRandomUnseen(DoubleTakeActors);

	if (IsValid(chosenObject) == true)
	{
		_Object = chosenObject;

		_StartLoc = _Object->GetActorLocation();
		_StartRot = _Object->GetActorRotation();

		_PeripheralViewBound = inPeripheralViewBound;
		_MaxDistanceFromOriginAllowed = inMaxDistanceFromOriginAllowed;

		_IsSpotted = false;

		SetActorTickEnabled(true);

		GetNewMoveDirection();

		return true;
	}

	return false;
}

//rotate the direction of movement to use for this object by a degree (chosen randomly)
void ADoubleTake::GetNewMoveDirection()
{
	int rotation = FMath::RandRange(0, 360);
	_CurrentMoveDirection = _CurrentMoveDirection.RotateAngleAxis(rotation, FVector(0, 0, 1));
}

//Called every frame
void ADoubleTake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If not spotted yet, update movement and check if spotted
	if (_IsSpotted == false)
	{
		MoveObject();
		CheckIfSpotted();
	}
	else //Has been spotted, work towards returning back
	{
		_TimeSinceReturnStart += DeltaTime;

		if (_TimeSinceReturnStart > _ReturnTime)
		{
			EndSymptom();
		}
		else
		{
			ReturnToOrigin();
		}
	}
}

//Move the object a little bit in the given direction, but not past the max distance from origin point allowed
void ADoubleTake::MoveObject()
{
	float distanceFromOrigin = FVector::Dist(_Object->GetActorLocation(), _StartLoc);

	if (distanceFromOrigin < _MaxDistanceFromOriginAllowed)
	{
		_Object->AddActorWorldOffset(_CurrentMoveDirection);
	}
}

//Check if object is in player's peripheral view. If so, initialize return to origin
void ADoubleTake::CheckIfSpotted()
{
	if (_Player->GetHorizontalDotProductTo(_Object) > _PeripheralViewBound || _Player->GetDotProductTo(_Object) > _PeripheralViewBound)
	{
		_IsSpotted = true;
		_TimeSinceReturnStart = 0.0f;
		_WorldLocationSpottedAt = _Object->GetActorLocation();

		//SPECIAL CASE: large difference in Y-locations (fallen off of shelf or other high place) = Snap back to position
		if (_StartLoc.Y > (_Object->GetActorLocation().Y + 20.0f))
		{
			EndSymptom();
		}
	}
}

//using a function of time, return the object back to its proper location
void ADoubleTake::ReturnToOrigin()
{
	FVector newLocation = _WorldLocationSpottedAt + (_TimeSinceReturnStart / _ReturnTime) * (_StartLoc - _WorldLocationSpottedAt);
	_Object->SetActorLocation(newLocation);
}

/** Reset the object's position and orientation and end the symptom */
void ADoubleTake::EndSymptom()
{
	_Object->SetActorLocation(_StartLoc);
	_Object->SetActorRotation(_StartRot);

	_Object = NULL;
	_CurrentMoveDirection = FVector(0, 0.25, 0);

	_IsSpotted = false;
	_TimeSinceReturnStart = 0.0f;
	_WorldLocationSpottedAt = FVector::ZeroVector;

	SetActorTickEnabled(false);
}