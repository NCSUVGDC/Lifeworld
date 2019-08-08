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

		*Return:	Original static mesh component, null if object isn't valid
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		UStaticMeshComponent* Shapeshift(AActor *object, UStaticMesh *mesh, UMaterialInterface *material);

	/*
		Returns a random actor from all valid actors in the scene

		*Return:  Random valid actor, null if no actors are valid
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		AActor* SelectRandomAll();

	/*
		Returns a random actor from all valid and currently unseen actors in the scene

		*Return:  Random valid actor that is not being viewed, null if no actors work
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		AActor* SelectRandomUnseen();
	
	/*
		Validates that an actor is unseen at the moment

		*Object:  The actor being checked if seen or not

		*Return:  True if actor is not being viewed, false if being viewed
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		bool ValidateUnseen(AActor *object);

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
