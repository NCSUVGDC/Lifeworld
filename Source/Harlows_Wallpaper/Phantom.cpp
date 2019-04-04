// Fill out your copyright notice in the Description page of Project Settings.

#include "Phantom.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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
/*	float dotProdH = player->GetHorizontalDotProductTo(this);
	float dotProd = player->GetDotProductTo(this);
	FString dpsh = "Hrz: " + FString::SanitizeFloat( dotProdH );
	FString dps = "Nml:" + FString::SanitizeFloat(dotProd);*/
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *dps);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, *dpsh);
	}*/


	if (!isSpotted && ( player->GetDotProductTo(this) >= 0.65 || player->GetHorizontalDotProductTo(this) >= 0.6) )
	{
		isSpotted = true;
		timeSpotted = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Spotted!"));
	}

	if (isSpotted)
	{
		if (player->GetHorizontalDotProductTo(this) > 0.75 || player->GetDotProductTo(this) > 0.75 || UGameplayStatics::GetRealTimeSeconds(GetWorld()) - timeSpotted > 3.0f)
		{
			isSpotted = false;
			SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
		//	if (GEngine)
		//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Bye Bye!"));

		}
	}

}


void APhantom::SetPlayer(AActor * actor)
{
	player = actor;
}
