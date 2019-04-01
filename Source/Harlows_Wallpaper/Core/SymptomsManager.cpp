// Fill out your copyright notice in the Description page of Project Settings.

#include "SymptomsManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"

// Sets default values
ASymptomsManager::ASymptomsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASymptomsManager::BeginPlay()
{
	Super::BeginPlay();

	// Set a custom tick rate
	if (TickInterval > 0)
		SetActorTickInterval(TickInterval);

	UE_LOG(LogTemp, Log, TEXT("%d symptoms available"), USymptomUtilitiesManager::SymptomDetails.Num());
	
	// Handle actors in world with pre-assigned Symptoms via the Tag system
	// This is done once here on BeginPlay; any future symptoms should be added via AddSymptomToActor
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	// Add actors to symptom array
	for (AActor* SymptomActor : FoundActors)
	{
		// Check for active symptoms
		if (SymptomActor->Tags.Num() != 0)
		{
			// Loop backwards over the list so we can safely remove tags as we iterate
			for (int SymptomIdx = SymptomActor->Tags.Num() - 1; SymptomIdx >= 0; SymptomIdx--)
			{
				FName& Symptom = SymptomActor->Tags[SymptomIdx];

				//check if the tag says it can be moved by DoubleTakeSymptom
				if (Symptom == FName("Possess"))
				{
					DoubleTakeActors.Add( SymptomActor );
				}
				else
				{
					bool SuccessfullyAddedSymptom = AddSymptomToActor(SymptomActor, Symptom);
					if (SuccessfullyAddedSymptom)
					{
						// No point in keeping the tag now that the symptom has been applied
						SymptomActor->Tags.RemoveAt(SymptomIdx);
					}
				}
			}
		}
	}
}

// Called every frame
void ASymptomsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tick symptom durations
	UpdateActiveSymptoms(DeltaTime);

	// run symptom effects
	for (int32 i = SymptomActors.Num() - 1; i >= 0; --i)
	{
		FSymptom *Symptom = &(SymptomActors[i]);

		// The inner most parentheses with `SymptomFunctions[Symptom->SymptomEffectIndex]` gets the
		// symptom function pointer; the `this->*` dereferences that function pointer and calls it;
		// The finally parentheses with `Symptom->SymptomActor` are the function args
		(this->* (SymptomFunctions[Symptom->SymptomEffectIndex]))(Symptom->SymptomActor);
	}
}

bool ASymptomsManager::AddSymptomToActor(AActor* Actor, const FName Symptom)
{
	// Find corresponding duration and effect for Symptom
	FSymptomDetails *Details = USymptomUtilitiesManager::SymptomDetails.Find(Symptom);

	// Dereferencing null pointers causes a hard crash - best to catch it here and keep an eye on the log
	if (Details == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find symptom details for symptom '%s'! Could not add to actor '%s'!"), 
			*Symptom.ToString(), *Actor->GetName());
		return false;
	}

	// Add symptom and remove tags
	SymptomActors.Add(FSymptom(Actor, Details->SymptomDuration, Details->SymptomEffectIndex));
	UE_LOG(LogTemp, Warning, TEXT("Added %s to active symptom actors"), *(Actor->GetName()));
	return true;
}

void ASymptomsManager::UpdateActiveSymptoms(float DeltaTime)
{
	// Check if it's safe to log; set CanTickThisFrame to false in case it was true last frame
	DebugSymptomCanTickThisFrame = false;
	DebugSymptomTickCounter += DeltaTime;
	if (DebugSymptomTickCounter >= DebugSymptomTickFrequency)
	{
		DebugSymptomCanTickThisFrame = true;
		DebugSymptomTickCounter -= DebugSymptomTickFrequency;
	}

	// We'll be decrementing by the same amount for each symptom
	FTimespan Decrement = FTimespan::FromSeconds(DeltaTime);

	// Log the decrement time if there are any symptomsto decrement
	if (SymptomActors.Num() > 0)
		UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Log, TEXT("Decrementing all symptoms by %s"), *Decrement.ToString());

	for (int32 SymptomIdx = SymptomActors.Num() - 1; SymptomIdx >= 0; SymptomIdx--)
	{
		// Decrement duration
		FSymptom* Symptom = &SymptomActors[SymptomIdx];
		Symptom->Duration -= Decrement;
		UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Log, TEXT("%f seconds remaining for active symptom in %s"), 
			Symptom->Duration.GetTotalSeconds(), *(Symptom->SymptomActor->GetName()));

		// Remove if expired
		if (Symptom->Duration.GetTotalSeconds() <= 0.0)
		{
			// Log about expirations regardless of the tick frequency; that's pertinent info.
			UE_LOG(LogTemp, Warning, TEXT("%s with Symptom ID %d expired"), *(Symptom->SymptomActor->GetName()), Symptom->SymptomEffectIndex);
			SymptomActors.RemoveAt(SymptomIdx); // remove expired symptom
		}
	}
}

/* Symptom Functions */
void ASymptomsManager::ImposeSizePerception(AActor * SymptomActor)
{
	if (DebugSymptomCanTickThisFrame && GEngine)
	{
		// just print that we're executing successfully, for now
		FString DebugMsg = FString::Printf(TEXT("Running SizePerception Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}
	
	UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Warning, TEXT("Running SizePerception Symptom on Actor %s"), *SymptomActor->GetName());
	// TODO: actor's size changes
}

void ASymptomsManager::ImposeVoices(AActor * SymptomActor) {}

void ASymptomsManager::ImposeDoubleTake(AActor * SymptomActor)
{

	if (DebugSymptomCanTickThisFrame && GEngine)
	{
		// just print that we're executing successfully, for now
		FString DebugMsg = FString::Printf(TEXT("Running DoubleTake Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}

	UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Warning, TEXT("Running DoubleTake Symptom on Actor %s"), *SymptomActor->GetName());
	
	//Tells if we found a suitable StaticMeshActor to move with this symptom
	bool foundActor = false;

	//If we dont find a StaticMeshActor within player's periphery, this will allow us to at least possess the closest thing
	AActor* closestObject = DoubleTakeActors[0];
	float closestDot = -1;

	// Iterate through list of possessable objects
	for (AActor* object : DoubleTakeActors)
	{
		float dotProd = (GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager)->GetDotProductTo(object);
		//If object is in periphery, possess with this symptom
		if (dotProd >= 0.7 && dotProd < 0.8)
		{
			//Spawn a new PossessObject and tie both player and object to it
			APossessedObject* pj = (APossessedObject*)GetWorld()->SpawnActor(APossessedObject::StaticClass());

			pj->setObject(object);
			pj->setPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			//Initiate movement in  possessed object
			pj->DoASpoopyThing();
			foundActor = true;
			break;
		}
		//If not in periphery, check if it is closer to player's view than current "closestObject"
		else if ( dotProd > closestDot && dotProd < .8 )
		{
			closestObject = object;
			closestDot = dotProd;
		}
	}

	//If an actor was not found in player's periphery, possess what was deemed the closest object
	if (!foundActor)
	{
		//Spawn a new PossessObject and tie player and object to it
		APossessedObject* pj = (APossessedObject*)GetWorld()->SpawnActor(APossessedObject::StaticClass());

		pj->setObject(closestObject);
		pj->setPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
		pj->DoASpoopyThing();
		foundActor = true;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Running Double-Take!"));
	}
}

void ASymptomsManager::ImposeWarpingWalls(AActor * SymptomActor) {}

void ASymptomsManager::ImposePhantom(AActor * SymptomActor)
{
	if (DebugSymptomCanTickThisFrame && GEngine)
	{
		// just print that we're executing successfully, for now
		FString DebugMsg = FString::Printf(TEXT("Running Phantom Symptom on Actor %s"), *SymptomActor->GetName());
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, DebugMsg);
	}

	UE_CLOG(DebugSymptomCanTickThisFrame, LogTemp, Warning, TEXT("Running Phantom Symptom on Actor %s"), *SymptomActor->GetName());

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
		//Get reference to phantom
		//Find the phantom that exists in the world and store a reference to it
		TArray<AActor*> GhostsNStuff;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);

		//If we have the one phantom found, store the reference at index 0 of TArray
		if (GhostsNStuff.Num() == 1)
		{
			phantom = (APhantom*)GhostsNStuff[0];
			phantom->SetActorHiddenInGame(false);
			//Set phantom's reference to player
			phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			//Spawn phantom at end of line from LineTrace
			FVector ghostLoc = ((RightVector * 80.f) + Start);
			ghostLoc.Z = 0;
			phantom->SetActorLocation(ghostLoc);
			phantom->SetActorTickEnabled(true);
			//Rotate phantom to face player
			FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(phantom->GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
			facePlayer.Roll = 0;
			facePlayer.Pitch = 0;
			facePlayer.Yaw -= 90;
			phantom->SetActorRotation(facePlayer);
			return;
		}
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
		//Get reference to phantom
		//Find the phantom that exists in the world and store a reference to it
		TArray<AActor*> GhostsNStuff;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);

		//If we have the one phantom found, store the reference at index 0 of TArray
		if (GhostsNStuff.Num() == 1)
		{
			phantom = (APhantom*)GhostsNStuff[0];
			phantom->SetActorHiddenInGame(false);
			//Get reference to player and pass to phantom
			phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			//Get location of end of line trace, spawn phantom there
			FVector ghostLoc = ((LeftVector * 80.f) + Start);
			ghostLoc.Z = 0;
			phantom->SetActorLocation(ghostLoc);
			phantom->SetActorTickEnabled(true);
			//Make phantom face player
			FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(phantom->GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
			facePlayer.Roll = 0;
			facePlayer.Pitch = 0;
			facePlayer.Yaw -= 90;
			phantom->SetActorRotation(facePlayer);
		}
	}
}

void ASymptomsManager::ImposeBackIsTurned(AActor * SymptomActor) {}

void ASymptomsManager::ImposeLensFlaring(AActor * SymptomActor) {}

void ASymptomsManager::ImposeBreatheIn(AActor * SymptomActor) {}

void ASymptomsManager::ImposeFallingFloor(AActor * SymptomActor) {}

