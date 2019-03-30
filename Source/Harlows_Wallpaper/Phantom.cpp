// Fill out your copyright notice in the Description page of Project Settings.

#include "Phantom.h"
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

	float num = player->GetDotProductTo(this);
	FString TheFloatStr = FString::SanitizeFloat(num);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);

	if (!isSeen && player->GetDotProductTo(this) >= 0.5)
	{
		isSeen = true;
		timeSpotted = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Spotted!"));
	}

	if (isSeen)
	{
		if (player->GetDotProductTo(this) > 0.75 || UGameplayStatics::GetRealTimeSeconds(GetWorld()) - timeSpotted > 3.0f)
		{
			isSeen = false;
			//SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Bye Bye!"));

		}
	}

}

void APhantom::SetPlayer(AActor * actor)
{
	player = actor;
}
