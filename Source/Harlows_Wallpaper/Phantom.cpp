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
	SetActorHiddenInGame(true);
	//SetActorTickEnabled(false);
	isSpotted = false;
	isRunning = false;

}

// Called every frame
void APhantom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, FString::Printf(TEXT("%f"), DeltaTime));


	if (DeltaTime > timeToSpawnAt)
	{
    	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Turquoise, TEXT("Spawn new phantom"));
		bool success = SymptomManager->AddSymptomToActor(this, "Symptoms.Phantom");
		if (success)
		{

		//	SpawnPhantom();
			timeToSpawnAt += 20.0f;
		}

	}

	if ( isRunning )
	{
		tickCount++;
		if (tickCount > 630)
		{
			isSpotted = false;
			isRunning = false;
			SetActorHiddenInGame(true);
			tickCount = 0;
		}
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
	if (!isRunning && !isSpotted )
	{
		SpawnPhantom();
	}
	else if (!isSpotted)
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


void APhantom::SpawnPhantom()
{
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
		isRunning = true;
	}
	
}

void APhantom::EndSymptom()
{
	isRunning = false;
	isSpotted = false;
	SetActorHiddenInGame(true);
}
