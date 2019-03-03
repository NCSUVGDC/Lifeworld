// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleFloor.h"


// Sets default values
ADestructibleFloor::ADestructibleFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AActor::SetActorTickEnabled(false);


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);


	/**
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> MeshContainer(TEXT("DestructibleMesh'/Game/HarlowsWallpaper/Environment/floor/ph_chess_board_DM.ph_chess_board_DM'"));
	if (MeshContainer.Succeeded())
	{
		UDestructibleMesh* mesh = MeshContainer.Object;
		DestructibleComponent->SetDestructibleMesh(mesh);
	}
	*/

	bCanBeDamaged = false;
	destroyed = false;

}

void ADestructibleFloor::FallToRuin()
{
	static FName FallingTag = FName(TEXT("FloorTrace"));
	FCollisionQueryParams TraceParams(FallingTag, true, Instigator);
	TArray<FHitResult> hits;
	FVector startTrace = GetActorLocation();
	FVector endTrace = startTrace + FVector(0, 0, 300);

	//Remember initial position before being destroyed
	position = DestructibleComponent->GetComponentLocation();
	bCanBeDamaged = true;

	DestructibleComponent->ApplyDamage(1000, position, FVector(0, 0, -10), 100);
	destroyed = true;

}

void ADestructibleFloor::Restore()
{
	bCanBeDamaged = false;
	DestructibleComponent->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));

	//Resetting destrucible mesh restores its state
	DestructibleComponent->SetDestructibleMesh(DestructibleComponent->GetDestructibleMesh());
	DestructibleComponent->SetWorldLocation(position);
	destroyed = false;
	for (int i = 0; i < size; i++) {
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Yellow, FString("Location: " + fallers[i]->GetActorLocation().ToString() + " | " + "Rotation: " + fallers[i]->GetActorRotation().ToString() + " | " + "Scale: " + fallers[i]->GetActorScale().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString("Location: " + previousTrans[i].GetLocation().ToString() + " | " + "Rotation: " + previousTrans[i].GetRotation().ToString() + " | " + "Scale: " + previousTrans[i].GetScale3D().ToString()));
		fallers[i]->SetActorTransform(previousTrans[i]);
	}
}

// Called when the game starts or when spawned
void ADestructibleFloor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADestructibleFloor::Tick(float DeltaTime)
{
	if (destroyed) {
		elapsedTime += DeltaTime;
		Super::Tick(DeltaTime);
		if (elapsedTime >= forceRestore) {
			Restore();
		}
		else if (elapsedTime >= restoreStart) {
			APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();
			FVector forward = player->GetActorForwardVector();
			FVector loc = player->GetActorLocation();
			float f = FVector::DotProduct(GetActorLocation() - loc, forward);
			if (f < 0) {
				Restore();
			}
		}
	}
}

void ADestructibleFloor::setTick(bool b)
{
	PrimaryActorTick.bCanEverTick = b;
}

