// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shapeshifting.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API AShapeshifting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShapeshifting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
		Turns an object's mesh and material into a new mesh and material

		*Object:	The shapeshifted actor
		*Mesh:		The new mesh
		*Material:	The new material

		*Return:	Original static mesh component, null if shapeshift doesn't occur
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		UStaticMeshComponent* Shapeshift(AActor *object, UStaticMesh *mesh, UMaterialInterface *material);

	/*
		Gets an array of actors that can be used for Shapeshift

		*Return:	Array of actors that are valid for Shapeshift
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		TArray<AActor*> getShapeshiftActors() const;

private:
	// Array of actors that can be accessed by symptom
	UPROPERTY()
		TArray<AActor*> validActors;

	// Player pawn
	UPROPERTY()
		APawn *lifeworldPlayer;

	// Camera component of player pawn
	UPROPERTY()
		USceneComponent *lifeworldCamera;
};
