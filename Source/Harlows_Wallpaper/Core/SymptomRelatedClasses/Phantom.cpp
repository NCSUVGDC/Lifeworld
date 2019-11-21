// Fill out your copyright notice in the Description page of Project Settings.

#include "Phantom.h"
#include "DrawDebugHelpers.h"
#include "Core/SymptomsManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


// Sets default values
APhantom::APhantom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APhantom::BeginPlay()
{
	Super::BeginPlay();

	//Make sure phantom can't be seen 
	//SetActorHiddenInGame(true);

	//Set phantom's reference to player
	SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
}

//Set refence to player
void APhantom::SetPlayer(AActor * actor)
{
	player = actor;
}


// Called every frame
void APhantom::Tick(float DeltaTime)
{
	float currentSecond = TimeSystem->CurrentSecond();

	Super::Tick(DeltaTime);

	Update();
}


//Called from symptom manager, allows Phantom to update state by calculating location based on player
//And disappear when needed to
void APhantom::Update()
{
	if (isRunning)
	{
		//If phantom is not spotted yet, make the dot product check to see if player is looking at it
		if (!isSpotted)
		{
			//Because of the nature of the VR headset, we must calculate two different dot products to get the best estimation of the player's view
			//to the phantom. This statement checks if either is exceeding the value that would indicate the phantom is well within view of the player
			if (player->GetDotProductTo(this) >= _PeripheralViewBound || player->GetHorizontalDotProductTo(this) >= _PeripheralViewBound)
			{
				//Phantom has been spotted, start the clock until it should automatically disappear
				isSpotted = true;
				timeSpotted = TimeSystem->CurrentSecondFloat();

				FTimerHandle timerHandle;

				GetWorldTimerManager().SetTimer(timerHandle, this, &APhantom::EndSymptom, _ViewTimeAllowed, false);
			}
		}
		//otherwise...
		else
		{
			//Now check the dot products using values that would indicate the player is looking directly at the phantom.
			//if one of the dot products exceeds this number then make phantom disappear
			if (player->GetHorizontalDotProductTo(this) > _DirectViewBound || player->GetDotProductTo(this) > _DirectViewBound || TimeSystem->CurrentSecondFloat() - timeSpotted > _ViewTimeAllowed)
			{
				EndSymptom();
			}
		}
	}
}

/** Activate the phantom */
void APhantom::ActivatePhantom(int inKillTime, float inPeripheralViewBound, float inDirectViewBound, float inViewTimeAllowed)
{
	FString infinity = "inf";

	SetActorTickEnabled(true);

	_KillTime = inKillTime;

	_ViewTimeAllowed = 1000;

	_PeripheralViewBound = inPeripheralViewBound;

	_DirectViewBound = inDirectViewBound;

	_ViewTimeAllowed = inViewTimeAllowed;

	FindPhantomSpawn();

	if (isRunning == false)
	{
		FTimerHandle timerHandle;

		GetWorldTimerManager().SetTimer(timerHandle, this, &APhantom::FindPhantomSpawn, 3.0f, false);
	}
}

void APhantom::FindPhantomSpawn()
{
	bool canSpawnRight = false;
	bool canSpawnLeft = false;

	FVector ghostLocLeft;
	FVector ghostLocRight;

	timeOfLastSpawnAttempt = TimeSystem->CurrentSecond();

	//Generate FHitResult and CollisionParams that will be needed for the LineTrace
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	//Calculate world position + rotation of Camera (Player's headset)
	FVector Start = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	//Set the height near the floor
	Start.Z = 17;

	//Get vector of player's right side + calculate end point of line for LineTrace
	FVector RightVector = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransformComponent()->GetRightVector();
	FVector End = ((RightVector * 90.f) + Start);
	End.Z = 17;

	//Send out line trace to see if there is an object within a certain distance of the player
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	//If nothing is hit, good to spawn phantom at end of line
	if (!isHit)
	{
		//cache location ghost can spawn at
		canSpawnRight = true;
		ghostLocRight = ((RightVector * 80.f) + Start);
		ghostLocRight.Z = 0;
	}

	//Right side was obstructed, so try spawning on left side
	//Flip right vector + draw new linetrace
	FVector LeftVector = -RightVector;
	End = ((LeftVector * 90.f) + Start);
	End.Z = 17;
	isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	//If nothing is hit on left side, spawn phantom at end of LineTrace
	if (!isHit)
	{
		//cache location ghost can spawn at
		canSpawnLeft = true;
		FVector ghostLocLeft = ((LeftVector * 80.f) + Start);
		ghostLocLeft.Z = 0;
	}

	if (canSpawnRight)
	{
		if (canSpawnLeft)
		{
			float val = FMath::RandRange(0.f, 1.f);
			val > 0.5 ? SpawnPhantom(ghostLocLeft) : SpawnPhantom(ghostLocRight);
		}
		else
		{
			SpawnPhantom(ghostLocRight);
		}
	}
	else if (canSpawnLeft)
	{
		SpawnPhantom(ghostLocLeft);
	}

}

void APhantom::SpawnPhantomWithLocation(FVector inSpawnLoc, int inKillTime, float inPeripheralViewBound, float inDirectViewBound, float inViewTimeAllowed)
{
	ActivatePhantom(inKillTime, inPeripheralViewBound, inDirectViewBound, inViewTimeAllowed);

	SpawnPhantom(inSpawnLoc);
}

void APhantom::SpawnPhantom(FVector spawnLoc)
{
	//Place phantom in new location
	SetActorLocation(spawnLoc);

	//Rotate phantom to face player
	FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
	facePlayer.Roll = 0;
	facePlayer.Pitch = 0;
	facePlayer.Yaw -= 90;
	SetActorRotation(facePlayer);

	isRunning = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void APhantom::EndSymptom()
{
	UE_LOG(LogTemp, Warning, TEXT("ending symptom"));

	isRunning = false;
	isSpotted = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}