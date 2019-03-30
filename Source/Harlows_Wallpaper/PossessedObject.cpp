// Fill out your copyright notice in the Description page of Project Settings.

#include "PossessedObject.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
APossessedObject::APossessedObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	float num = player->GetDotProductTo(object);
	FString TheFloatStr = FString::SanitizeFloat(num);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *TheFloatStr);

	if (player->GetDotProductTo(object) >= .75)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Return to Normal"));
		//object->SetActorLocation(startLoc);
		//object->SetActorRotation(startRot);
		Destroy();
	}
	else
	{
		//object->AddActorWorldOffset(FVector(5, 0, 0));
		//object->AddActorWorldRotation(FRotator(0,0,5));
	}

	/*if (startLoc != FVector::ZeroVector)
	{
	float num = player->GetDotProductTo(object);
	FString TheFloatStr = FString::SanitizeFloat(num);
	if (GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, *TheFloatStr);

	if (player->GetDotProductTo(object) >= .75)
	{
	object->SetActorLocation(startLoc);
	Destroy();
	}

	}*/

}

void APossessedObject::setObject(AActor * obj)
{
	object = obj;
	startLoc = obj->GetActorLocation();
	startRot = obj->GetActorRotation();
}

void APossessedObject::setPlayer(AActor * ply)
{
	player = ply;
}

void APossessedObject::DoASpoopyThing()
{
	FVector force = FVector(0, 0, 10000);

	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(object->GetRootComponent());



	if (mesh)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("SPOOPY"));
		mesh->AddForce(FVector::RightVector * 10000, NAME_None, true);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cast Failed"));
	}

	//mesh->AddForce(force * mesh->GetMass() );

	/*	FVector Orgin;
	FVector BoundsExtent;
	object->GetActorBounds(false, Orgin, BoundsExtent);

	FVector point = UKismetMathLibrary::RandomPointInBoundingBox(object->GetActorLocation(), BoundsExtent);
	*/

	//mesh->AddImpulseAtLocation(force, object->GetActorLocation() );
	//mesh->AddRadialImpulse( point, 10, 5000.0f, ERadialImpulseFalloff::RIF_Constant, true);
	//	mesh->AddAngularImpulse(force);

}

