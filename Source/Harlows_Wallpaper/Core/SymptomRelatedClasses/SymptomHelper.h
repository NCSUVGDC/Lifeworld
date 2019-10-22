// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymptomHelper.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ASymptomHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASymptomHelper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
		Returns a random actor from an array of actors

		*Actors:  Array of actors being chose from

		*Return:  Random valid actor, null if no actors are valid
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms Helpers")
		static AActor* SelectRandomAll(TArray<AActor*> actors);

	/*
		Returns a random actor from all unseen actors in an array

		*Actors:  Array of actors being chose from

		*Return:  Random valid actor that is not being viewed, null if no actors work
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms Helpers")
		static AActor* SelectRandomUnseen(TArray<AActor*> actors);

	/*
		Validates that an actor is unseen at the moment

		*Object:  The actor being checked if seen or not

		*Return:  True if actor is not being viewed, false if being viewed
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms Helpers")
		static bool ValidateUnseen(AActor *object);

	/*
		Validates that a location is unseen at the moment

		*Location:  The location vector being checked if seen or not
		*Object:	Any actor used to get a reference to the world

		*Return:	True if location is not being viewed, false if being viewed
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms Helpers")
		static bool ValidateUnseenLocation(FVector location, AActor *object);

	/*
		Creates a clone of an actor with the exact same properties

		*Object:  The original actor being cloned

		*Return:  The cloned actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms Helpers")
		static AActor* CloneActor(AActor *object);
	
private:
	// Nothing, because static class
};
