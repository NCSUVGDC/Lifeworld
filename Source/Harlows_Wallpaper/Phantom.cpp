// Fill out your copyright notice in the Description page of Project Settings.

#include "Phantom.h"
#include "DrawDebugHelpers.h"
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
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

// Called every frame
void APhantom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//End symptom
	tickCount++;

//	FString tickCount = " " + tickCount;
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, FString::Printf(TEXT("%d"), tickCount));

	if (tickCount > 630)
	{
		isSpotted = false;
		isRunning = false;
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
		tickCount = 0;
	}


}

//Set refence to player and make the phantom visible
void APhantom::SetPlayer(AActor * actor)
{
	player = actor;
	symptomTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	SetActorHiddenInGame(false);
}

//Called from symptom manager, allows Phantom to update state by calculating location based on player
//And disappearing if needed to
void APhantom::Update()
{
	// just print that we're executing successfully, for now


	//This is used to see if upon calling Update(), the symptom manager is doing it for the first time
	//The time check will tell if it has been awhile since the update() was last called, indicating 
	//time to select a new spawn position
	if (!isRunning)
	{
		SpawnPhantom();
	}
	else
	{
		//if the phantom has not been spotted yet...
		if (!isSpotted)
		{
			//Because of the nature of the VR headset, we must calculate two different dot products to get the best estimation of the player's view
			//to the phantom. This statement checks if either is exceeding the value that would indicate the phantom is well within view of the player
			if (player->GetDotProductTo(this) >= 0.65 || player->GetHorizontalDotProductTo(this) >= 0.6)
			{
				//Phantom has been spotted, start the clock until it should automatically disappear
				isSpotted = true;
				timeSpotted = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			}
		}
		//otherwise...
		else
		{
			//Now check the dot products using values (.75) that would indicate the player is looking directly at the phantom.
			//OR if it has been 3 seconds since first sighting, make phantom disappear
			if (player->GetHorizontalDotProductTo(this) > 0.75 || player->GetDotProductTo(this) > 0.75 || UGameplayStatics::GetRealTimeSeconds(GetWorld()) - timeSpotted > 3.0f)
			{
				isSpotted = false;
				SetActorHiddenInGame(true);
			}
		}
	}
}


void APhantom::SpawnPhantom()
{
/*	// just print that we're executing successfully, for now
	FString DebugMsg = FString::Printf(TEXT("Spawning Phantom"));
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, DebugMsg);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, DebugMsg);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, DebugMsg);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, DebugMsg);
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Orange, DebugMsg);*/
	//Generate FHitResult and CollisionParams that will be needed for the LineTrace
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	//Calculate world position + rotation of Camera (Player's headset)
	FVector Start = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	//Set the height near the floor
	Start.Z = 17;

	//Get vector of player's right side + calculate end of line for LineTrace
	FVector RightVector = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransformComponent()->GetRightVector();
	FVector End = ((RightVector * 90.f) + Start);
	End.Z = 17;


	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	//If nothing is hit, good to spawn phantom
	if (!isHit)
	{

		//Set phantom's reference to player
		SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
		//Spawn phantom at end of line from LineTrace
		FVector ghostLoc = ((RightVector * 80.f) + Start);
		ghostLoc.Z = 0;
		SetActorLocation(ghostLoc);
		//Rotate phantom to face player
		FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
		facePlayer.Roll = 0;
		facePlayer.Pitch = 0;
		facePlayer.Yaw -= 90;
		SetActorRotation(facePlayer);
		isRunning = true;
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
		return;
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
		//Get reference to player and pass to phantom
		SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
		//Get location of end of line trace, spawn phantom there
		FVector ghostLoc = ((LeftVector * 80.f) + Start);
		ghostLoc.Z = 0;
		SetActorLocation(ghostLoc);
		//Make phantom face player
		FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
		facePlayer.Roll = 0;
		facePlayer.Pitch = 0;
		facePlayer.Yaw -= 90;
		SetActorRotation(facePlayer);
		SetActorHiddenInGame(false);
		SetActorTickEnabled(true);
		isRunning = true;
	}
	
}

void APhantom::EndSymptom()
{
	isRunning = false;
	isSpotted = false;
	SetActorHiddenInGame(true);
}
