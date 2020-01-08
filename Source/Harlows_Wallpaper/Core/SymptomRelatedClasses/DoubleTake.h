// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/SymptomsManager.h"
#include "Core/Tween.h"
#include "SymptomHelper.h"
#include "Core/TimeSystem.h"
#include "DoubleTake.generated.h"

UCLASS()
class HARLOWS_WALLPAPER_API ADoubleTake : public AActor
{
	GENERATED_BODY()

private:

	const FString DoubleTakeTag = "DoubleTake";

	//Reference to object
	AActor* _Object;
	//Refence to player
	AActor* _Player;

	//Starting location + rotation of object
	FVector _StartLoc = FVector::ZeroVector;
	FRotator _StartRot = FRotator::ZeroRotator;

	//boundary of the player's peripheral view
	float _PeripheralViewBound = 0.7f;

	//Tells if the object has been spotted
	bool _IsSpotted = false;

	//X distance of object from original location
	float ddX;
	//Y distance of object from original location
	float ddY;

	//Number of movement iterations
	int iterationCount = 0;

	//Ends the symptom, immediately returns object to resting place
	void EndSymptom();

	//Array of actors that can be possess by double take function
	UPROPERTY()
		TArray<AActor*> DoubleTakeActors;

public:

	//Used to track movement of the object moved by this symptom as a unit of time
	Tween TweenInstance;

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

	//Update called once per frame by SymptomManager
	void Update();

	UPROPERTY(EditAnywhere)
		class ATimeSystem* TimeSystem;

   /* Select a possessable object outside of player's view and activate symptom on it 
	*
	*PeripheralViewBound:	how far the phantom is allowed to appear into the player's F.O.V before the timer for it to disappear begins (1 = direct line of sight)

	* return: True if compatible item was found, false if not
	*/
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		bool StartSymptom(float PeripheralViewBound = 0.7f);

	/* Gets an array of actors that can be used for DoubleTake
	 *
	 * Return:	Array of actors that are valid for DoubleTake
	 */
	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		TArray<AActor*> getDoubleTakeActors() const;

	UFUNCTION(BlueprintCallable, Category = "Symptoms")
		bool ChooseTargetLocation(float inMaxDistanceAllowed);
};