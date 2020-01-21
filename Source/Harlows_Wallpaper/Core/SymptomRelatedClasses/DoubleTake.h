// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SymptomsManager.h"
#include "SymptomHelper.h"
#include "Core/TimeSystem.h"
#include "DoubleTake.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ADoubleTake : public AActor
{
	GENERATED_BODY()

private:

	//tag used on objects that this symptom can possess
	const FString DoubleTakeTag = "DoubleTake";

	//Reference to object
	AActor* _Object;
	//Refence to player
	AActor* _Player;

	//Starting location + rotation of object
	FVector _StartLoc = FVector::ZeroVector;
	FRotator _StartRot = FRotator::ZeroRotator;

	//Direction the object being moved by this symptom is currently moving
	FVector _CurrentMoveDirection = FVector(0, 0.25, 0);

	//boundary of the player's peripheral view
	float _PeripheralViewBound = 0.7f;

	//maximum distance object is allowed to move from its origin
	float _MaxDistanceFromOriginAllowed;

	//Tells if the object has been spotted
	bool _IsSpotted = false;

	//Array of actors that can be possess by double take function
	TArray<AActor*> DoubleTakeActors;

	//total time the object being moved has to return to original spot after being spotted
	float _ReturnTime = 2.0f;

	//time passed since the object being moved was spotted
	float _TimeSinceReturnStart = 0.0f;

	//World location the object was when spotted by player
	FVector _WorldLocationSpottedAt;

private:

	//Get a random direction to move this object in for this symptom
	void GetNewMoveDirection();

	//Move the object a little bit
	void MoveObject();

	//Check if object is in player's peripheral view
	void CheckIfSpotted();

	//Moves object closer to its original spot based on time passed since being sighted by player
	void ReturnToOrigin();

	//Ends the symptom, immediately returns object to resting place
	void EndSymptom();


protected:

	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//Sets default values for this actor's properties
	ADoubleTake();

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Set refence to the player for easy access when calculating dot products
	void SetPlayer(AActor * ply);

	UPROPERTY(EditAnywhere)
		class ATimeSystem* TimeSystem;

   /* Select a possessable object outside of player's view and activate symptom on it 
	*
	*MaxDistanceFromOriginAllowed: In this new version, the actor is constantly moving. This determines the max distance the object is allowed to move from its origin
	*PeripheralViewBound:	how far the phantom is allowed to appear into the player's F.O.V before the timer for it to disappear begins (1 = direct line of sight)
	* return: True if compatible item was found, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		bool StartSymptom(float MaxDistanceFromOriginAllowed, float PeripheralViewBound = 0.7f);

	/* Gets an array of actors that can be used for DoubleTake
	 *
	 * Return:	Array of actors that are valid for DoubleTake
	 */
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		TArray<AActor*> getDoubleTakeActors() const;
};