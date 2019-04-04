// Fill out your copyright notice in the Description page of Project Settings.

#include "SizePerceptionActor.h"
#include "Harlows_Wallpaper/Player/HarlowPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include <random>


// Sets default values
ASizePerceptionActor::ASizePerceptionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASizePerceptionActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("SizePerceptionActor BeginPlay"));

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

				// Check if the tag says it can be scaled by SizePerception
				if (Symptom == FName("SizePerception"))
				{
					SizePerceptionActors.Add(SymptomActor);
					UE_LOG(LogTemp, Warning, TEXT("Added %s"), *SymptomActor->GetName());
					SymptomActor->Tags.Remove(FName("SizePerception"));
				}
			}
		}
	}

	// Sets proceed to a value that wouldn't cause problems
	proceed = false;

	// Initializes DotProducts to an array of length of SizePerceptionActors
	DotProducts.SetNum(SizePerceptionActors.Num());
}

// Called every frame
void ASizePerceptionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Causes any abnormally sized objects to scale to its original size as the player gets closer
	ScaleOriginal();

	// Gets the number of seconds since game started as an int
	float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	int32 realtimeSecondsInt = (int32)realtimeSeconds;
	
	// Checks if game time is during one of the trigger times for SizePerception
	switch (realtimeSecondsInt){
	case 10:
		if (proceed) {
			Scale(10);
		}
		break;
	case 19:
		tempScale = 1;	// Resets tempScale so things go back to normal
		break;
	case 20:
		if (proceed) {
			Scale(20);
		}
	break;
	//case 30:
	//	tempScale = 1;
	//	break;
	default:
		proceed = true;	// Reset proceed whenever it's not a trigger time
		break;
	}
}

// Locates the actor used in SizePerception
AActor* ASizePerceptionActor::Select()
{
	/// Checks that the method was activated
	UE_LOG(LogTemp, Warning, TEXT("SELECT() TURNED ON"));
	
	// Sets LifeWorldPlayer to HarlowPawn
	LifeWorldPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	/// Checks that LifeWorldPlayer is the correct pawn
	if (LifeWorldPlayer->Tags[0] == FName("Player")) {
		UE_LOG(LogTemp, Warning, TEXT("Got the player pawn correctly"));
	}

	// Sets LifeWorldCamera to the camera component of LifeWorldPlayer
	LifeWorldCamera = Cast<USceneComponent>(LifeWorldPlayer->GetComponentByClass(UCameraComponent::StaticClass()));

	// Returned actor that'll be manipulated
	AActor* SelectedActor = NULL;

	// Creates array of dot products based on the actors in the scene
	AActor* LoopActor;
	for (int i = 0; i < SizePerceptionActors.Num(); i++) {
		LoopActor = SizePerceptionActors[i];							// Sets LoopActor to next one in array
		FVector LoopActorLocation = LoopActor->GetActorLocation();		// Gets location of the LoopActor
		AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as actor
		FVector PlayerLocation = CameraAct->GetActorLocation();			// Gets player camera location
		FVector DistanceVector = LoopActorLocation - PlayerLocation;	// Gets distance vector between LoopActor and player camera
		DistanceVector = DistanceVector / DistanceVector.Size();		// Normalizes the distance vector (magnitude of 1)
		FVector CameraVector = LifeWorldCamera->GetForwardVector();		// Gets forward vector of the camera
		CameraVector = CameraVector / CameraVector.Size();				// Normalizes the forward vector of the camera (magnitude of 1)
		float dotProd = FVector::DotProduct(DistanceVector, CameraVector);	// Gets dot product of normalized distance vector and normalized camera vector

		/// States what the Dot Product of each LoopActor is
		UE_LOG(LogTemp, Log, TEXT("%s Dot Product: %f"), *LoopActor->GetName(), dotProd);
		
		// Puts the dot product in an array of Dot Products
		DotProducts[i] = dotProd;
	}

	// Initializes FinalSelectionRandomizer to an empty array
	FinalSelectionRandomizer.SetNum(0);

	// Initializes the largestDistance and DefaultActor variables
	float largestDistance = 0;
	AActor* DefaultActor = NULL;

	// Cycles through the array of dot products to determine which ones are both behind the player and far away enough to be significant
	for (int i = 0; i < DotProducts.Num(); i++) {
		if (DotProducts[i] < 0.0) {
			LoopActor = SizePerceptionActors[i];							// Sets LoopActor to next one in array
			FVector LoopActorLocation = LoopActor->GetActorLocation();		// Gets location of LoopActor
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as actor
			FVector PlayerLocation = CameraAct->GetActorLocation();			// Gets player camera location
			FVector DistanceVector = LoopActorLocation - PlayerLocation;	// Gets distance vector between LoopActor and player camera
			UE_LOG(LogTemp, Log, TEXT("Distance to %s : %f"), *LoopActor->GetName(), DistanceVector.Size()); /// States how far away it is to LoopActor
			
			// Sets the default actor to be the one furthest away if none are far away enough
			if (DistanceVector.Size() > largestDistance) {
				largestDistance = DistanceVector.Size();	// Sets new distance to beat
				DefaultActor = LoopActor;					// Sets DefaultActor to the current LoopActor
				UE_LOG(LogTemp, Warning, TEXT("DefaultActor became: %s"), *DefaultActor->GetName()); /// States that DefaultActor has been changed
			}

			// If distance is significant enough (100 centimeters away), add the actor to FinalSelectionRandomizer array
			if (DistanceVector.Size() >= 100) {
				FinalSelectionRandomizer.Add(LoopActor);	// Adds LoopActor to array
				UE_LOG(LogTemp, Warning, TEXT("Added to FinalSelectionRandomizer: %s"), *LoopActor->GetName()); /// States that LoopActor was added to array
			}
		}
	}

	// Randomly picks an actor from FinalSelectionRandomizer if applicable, picks DefaultActor if not
	if (FinalSelectionRandomizer.Num() > 0) {
		std::random_device rd;     // Only used once to initialize (seed) engine
		std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, FinalSelectionRandomizer.Num() - 1); // Guaranteed unbiased
		auto random_integer = uni(rng); // Picks random number
		AActor* ChosenThing = FinalSelectionRandomizer[random_integer];									// Uses random number to pick an actor
		UE_LOG(LogTemp, Warning, TEXT("Chosen thing is: %s"), *ChosenThing->GetName());					/// States what actor was randomly chosen
		SelectedActor = ChosenThing;																	// Sets the returned variable to chosen actor
		FVector SelectedActorLocation = SelectedActor->GetActorLocation();								// Gets location of SelectedActor
		AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;	// Gets player camera as actor
		FVector PlayerLocation = CameraAct->GetActorLocation();											// Gets location of player camera
		FVector SADistanceVect = SelectedActorLocation - PlayerLocation;								// Gets distance vector between SelectedActor and player camera
		SADistance = SADistanceVect.Size();																// Sets SADistance to size of the distance vector
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Going with the DefaultActor because nothing is far enough"));
		SelectedActor = DefaultActor;	// If there were no actors far enough to be randomized, DefaultActor will be returned
	}

	/// States what SelectedActor ultimately becomes
	if (SelectedActor != NULL) {
		UE_LOG(LogTemp, Error, TEXT("FINAL SELECTEDACTOR IS: %s"), *SelectedActor->GetName());
		FString DebugMsg(TEXT("FINAL SELECTED ACTOR IS: "));
		FString DebugSub(SelectedActor->GetName());
		DebugMsg.Append(DebugSub);
		if (GEngine) {
			for (int i = 0; i < 25; i++) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugMsg);
			}
		}
	}
	
	// Returns the actor that was decidedly best for Size Perception
	return SelectedActor;
}

// Scales an actor back to its original size
void ASizePerceptionActor::ScaleOriginal() {
	if (DuplicateMeshActor != NULL && SPActor != NULL) {
		FVector LAScale = DuplicateMeshActor->GetActorScale();
		if (LAScale.X > 1 && tempScale > 1) {							// If the current scale is higher than <1,1,1> & it was sized up...
			FVector LoopActorLocation = DuplicateMeshActor->GetActorLocation();	// Gets location of actor
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as an actor
			FVector PlayerLocation = CameraAct->GetActorLocation();		// Gets location of player camera
			FVector DistanceVect = LoopActorLocation - PlayerLocation;	// Gets distance vector between LoopActor and player camera
			float distance = DistanceVect.Size();						// Gets distance of distance vector

			float scaleUnit = 1 + (((scaleSize - 1) / 190) * (distance - 70));	// Calculates what the scaling would become at current distance

			if (scaleUnit < tempScale) {										// If the scaling would be lower than the current scaling...
				FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);	// Create a scale vector of new scaling
				DuplicateMeshActor->SetActorScale3D(NewScale);					// Scales the duplicate actor to the new scale vector
				tempScale = scaleUnit;											// Sets the current scaling to the new scaling
			}
		}
		else if (LAScale.X < 1 && tempScale < 1) {						// If the current scale is lower than <1,1,1> & it was sized down...
			FVector LoopActorLocation = DuplicateMeshActor->GetActorLocation();	// Gets location of actor
			AActor* CameraAct = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager; // Gets player camera as an actor
			FVector PlayerLocation = CameraAct->GetActorLocation();		// Gets location of player camera
			FVector DistanceVect = LoopActorLocation - PlayerLocation;	// Gets distance vector between LoopActor and player camera
			float distance = DistanceVect.Size();						// Gets distance of distance vector

			float scaleUnit = 1 + (((scaleSize - 1) / 190) * (distance - 70));	// Calculates what the scaling would become at current distance

			if (scaleUnit > tempScale) {										// If the scaling would be higher than the current scaling...
				FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);	// Create a scale vector of new scaling
				DuplicateMeshActor->SetActorScale3D(NewScale);					// Scales the duplicate actor to the new scale vector
				tempScale = scaleUnit;											// Sets the current scaling to the new scaling
			}
		}
		else {
			DuplicateMeshActor->SetActorScale3D(FVector(1, 1, 1));				// If neither apply, make the actor scaled at <1,1,1>
			SPActor->SetActorHiddenInGame(false);								// Reveals the original actor
			DuplicateMeshActor->GetAttachedActors(DupeActorAttached);			// Makes TArray of duplicate attached actors
			if (DupeActorAttached.Num() != 0) {
				for (int i = 0; i < DupeActorAttached.Num(); i++) {
					SPActorAttached[i]->SetActorHiddenInGame(false);			// Reveals the original attached actor
					DupeActorAttached[i]->Destroy();
				}
			}
			DuplicateMeshActor->Destroy();										// Destroys the duplicate
		}
	}
}

// Duplicates an actor and then scales it based on scaleSize and distance
void ASizePerceptionActor::Scale(int scalSize) {
	UE_LOG(LogTemp, Warning, TEXT("ACTIVATED ONLY ONCE"));
	SPActor = Select(); // The original instance of the actor that will be sized

	// If the actor exists...
	if (SPActor != NULL) {
		scaleSize = scalSize;
		tempScale = scaleSize; // Used later on to cause resizing

		SPActor->GetAttachedActors(SPActorAttached); // Makes TArray of attached actors
		SPActor->SetActorHiddenInGame(true); // Hides the original actor so it doesn't cause problems

		// Creates duplicate actor that will be sized
		FActorSpawnParameters SpawnParams;
		DuplicateMeshActor = GetWorld()->SpawnActorAbsolute<AActor>(SPActor->GetClass(), SPActor->GetTransform(), SpawnParams);

		// Gets information about the original actor's mesh
		TArray<UStaticMeshComponent*> Components;						// Original actor USMC: Declares array of USMC
		SPActor->GetComponents<UStaticMeshComponent>(Components);		// Original actor USMC: Sets array of USMC
		UStaticMeshComponent* meshBoiComp = Components[0];				// Original actor USMC: Initializes variable to USMC in array
		UStaticMesh* meshBoiMesh = meshBoiComp->GetStaticMesh();		// Original actor Static Mesh: Gets Static Mesh of USMC
		UMaterialInterface* materialBoi = meshBoiComp->GetMaterial(0);	// Original actor Material

		// Applies information to duplicate actor
		TArray<UStaticMeshComponent*> DupeComponents;								// Duplicate actor USMC: Declares array of USMC
		DuplicateMeshActor->GetComponents<UStaticMeshComponent>(DupeComponents);	// Duplicate actor USMC: Sets array of USMC
		UStaticMeshComponent* dupeMeshBoiComp = DupeComponents[0];					// Duplicate actor USMC: Initializes variable to USMC in array

		// Duplicate actor ignore collisions
		if (UPrimitiveComponent* PrimitiveComponent = DuplicateMeshActor->FindComponentByClass<UPrimitiveComponent>())
		{
			PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		}

		//dupeMeshBoiComp->SetSimulatePhysics(false);									// Duplicate actor disable physics (MIGHT ACTUALLY NOT BE NEEDED)
		dupeMeshBoiComp->SetMobility(EComponentMobility::Movable);					// Duplicate actor allows Static Mesh
		dupeMeshBoiComp->SetStaticMesh(meshBoiMesh);								// Duplicate actor Static Mesh
		dupeMeshBoiComp->SetMaterial(0, materialBoi);								// Duplicate actor Material

		UPrimitiveComponent* ActorRootComponent = Cast<UPrimitiveComponent>(DuplicateMeshActor->GetRootComponent());
		ActorRootComponent->SetGenerateOverlapEvents(true);

		// If SPActor has attached actors...
		if (SPActorAttached.Num() != 0) {
			for (int i = 0; i < SPActorAttached.Num(); i++) {

				AActor* loopyActor = SPActorAttached[i];	// Refers to the original attached actor in the corresponding index of the array
				loopyActor->SetActorHiddenInGame(true);		// Hides the original attached actor so it doesn't cause problems

				UE_LOG(LogTemp, Warning, TEXT("Actor name is %s"), *loopyActor->GetName()); /// Name of attached actor being changed

				// Creates duplicate attached actor that will be sized
				FActorSpawnParameters SpawnParams;
				AActor* DuplicateMeshActorAttach = GetWorld()->SpawnActorAbsolute<AActor>(loopyActor->GetClass(), loopyActor->GetTransform(), SpawnParams);
				
				// Gets information about the original attached actor's mesh
				TArray<UStaticMeshComponent*> Components;						// Original actor USMC: Declares array of USMC
				loopyActor->GetComponents<UStaticMeshComponent>(Components);	// Original actor USMC: Sets array of USMC
				UStaticMeshComponent* meshBoiComp = Components[0];				// Original actor USMC: Initializes variable to USMC in array
				UStaticMesh* meshBoiMesh = meshBoiComp->GetStaticMesh();		// Original actor Mesh: Gets Static Mesh of USMC
				UMaterialInterface* materialBoi = meshBoiComp->GetMaterial(0);	// Original actor Material

				// Applies information to duplicate actor
				TArray<UStaticMeshComponent*> DupeComponents;									// Duplicate actor USMC: Declares array of USMC
				DuplicateMeshActorAttach->GetComponents<UStaticMeshComponent>(DupeComponents);	// Duplicate actor USMC: Sets array of USMC
				UStaticMeshComponent* dupeMeshBoiComp = DupeComponents[0];						// Duplicate actor USMC: Initializes variable to USMC in array

				// Duplicate attached actor ignore collisions
				if (UPrimitiveComponent* PrimitiveComponent = DuplicateMeshActorAttach->FindComponentByClass<UPrimitiveComponent>())
				{
					PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
				}

				//dupeMeshBoiComp->SetSimulatePhysics(false);								// Duplicate actor disable physics (MIGHT ACTUALLY NOT BE NEEDED)
				dupeMeshBoiComp->SetMobility(EComponentMobility::Movable);				// Duplicate actor allows Static Mesh
				dupeMeshBoiComp->SetStaticMesh(meshBoiMesh);							// Duplicate actor Static Mesh
				dupeMeshBoiComp->SetMaterial(0, materialBoi);							// Duplicate actor Material

				// Attaches duplicate attached actor to the duplicate main actor and sets it location to the original attached actor
				FHitResult* RV_Hit = new FHitResult(ForceInit);
				DuplicateMeshActorAttach->AttachToActor(DuplicateMeshActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				DuplicateMeshActorAttach->SetActorLocation(loopyActor->GetActorLocation(), false, RV_Hit, ETeleportType::TeleportPhysics);
				
				///The stuff below is the beginning of implementing a SkeletalMesh check in case we want the Flipclock to change size

				// Creates duplicate attached actor and snaps to DuplicateMeshActor
				//AActor* DuplicateMeshAttach = GetWorld()->SpawnActorAbsolute<AActor>(SPActorAttached[i]->GetClass(), SPActorAttached[i]->GetTransform(), SpawnParams);
				//DuplicateMeshAttach->AttachToActor(DuplicateMeshActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

				// Gets information about original attached actor
				//bool hasStaticMesh;
				//TArray<UStaticMeshComponent*> ComponentsAttach;									// Original attached actor USMC: Declares array of USMC
				//SPActorAttached[i]->GetComponents<UStaticMeshComponent>(ComponentsAttach);		// Original attached actor USMC: Sets array of USMC
				//if (ComponentsAttach.Num() != 0) {												// If original attached actor has a Static Mesh component...
				//	UE_LOG(LogTemp, Warning, TEXT("IT GRABBED THE ORIGINAL STUFF"));
				//	UStaticMeshComponent* meshBoiComp = ComponentsAttach[0];					// Original attached actor USMC: Initializes variable to USMC in array
				//	hasStaticMesh = true;														// States that it has a Static Mesh, not a vanilla Mesh
				//	UStaticMesh* meshBoiMesh = meshBoiComp->GetStaticMesh();					// Original attached actor StaticMesh
				//}
				//else {	// For clock																// If original attached actor does not have a Static Mesh component...
				//	TArray<USkeletalMeshComponent*> Components;										// Original attached actor UMC: Declares array of UMC
				//	SPActorAttached[i]->GetComponents<USkeletalMeshComponent>(Components);			// Original attached actor UMC: Sets array of UMC
				//	USkeletalMeshComponent* meshBoiComp = Components[0];							// Original attached actor UMC: Initializes variable to UMC in array
				//	hasStaticMesh = false;															// States that it has a vanilla Mesh, not a Static Mesh
				//	USkeletalMesh* meshBoiMesh = meshBoiComp->GetSkeletalMesh();					// Original attached actor Mesh
				//}
				//UMaterialInterface* materialBoi = meshBoiComp->GetMaterial(0);				// Original attached actor Material

				// Applies information to duplicate attached actor
				//if (UPrimitiveComponent* PrimitiveComponent = SPActorAttached[i]->FindComponentByClass<UPrimitiveComponent>())
				//{
				//	PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	// Duplicate attached actor ignore collisions
				//}
				//if (hasStaticMesh) {
				//	UE_LOG(LogTemp, Warning, TEXT("IT APPLIED THE ORIGINAL STUFF"));
				//	TArray<UStaticMeshComponent*> DupeComponentsAttach;								// Duplicate attached actor USMC: Declares array of USMC
				//	SPActorAttached[i]->GetComponents<UStaticMeshComponent>(DupeComponentsAttach);	// Duplicate attached actor USMC: Sets array of USMC
				//	UStaticMeshComponent* dupeMeshBoiComp = DupeComponentsAttach[0];				// Duplicate attached actor USMC: Initializes variable to USMC in array
				//	dupeMeshBoiComp->SetMobility(EComponentMobility::Movable);						// Duplicate attached actor allows Mesh
				//	dupeMeshBoiComp->SetStaticMesh(meshBoiMesh);									// Duplicate attached actor Mesh
				//}
				//else { // For clock
				//	TArray<USkeletalMeshComponent*> DupeComponents;									// Duplicate attached actor UMC: Declares array of USMC
				//	SPActorAttached[i]->GetComponents<USkeletalMeshComponent>(DupeComponents);		// Duplicate attached actor UMC: Sets array of USMC
				//	USkeletalMeshComponent* dupeMeshBoiComp = DupeComponents[0];					// Duplicate attached actor UMC: Initializes variable to USMC in array
				//	dupeMeshBoiComp->SetMobility(EComponentMobility::Movable);						// Duplicate attached actor allows Mesh
				//	dupeMeshBoiComp->SetSkeletalMesh(meshBoiMesh);									// Duplicate attached actor Mesh
				//}
				
				
				//dupeMeshBoiComp->SetSimulatePhysics(false);									// Duplicate attached actor disable physics
				//dupeMeshBoiComp->SetMaterial(0, materialBoi);									// Duplicate attached actor Material
			}
		}

		// Grabs distance from chosen object
		float distance = SADistance;
		UE_LOG(LogTemp, Warning, TEXT("Distance to Scaled Item: %f"), distance);

		// Scaling based on scaleSize and distance
		float scaleUnit = 1 + (((scaleSize - 1) / 190) * (distance - 70));	// Calculates new values that scale will size to
		FVector NewScale = FVector(scaleUnit, scaleUnit, scaleUnit);		// Creates new scale vector
		DuplicateMeshActor->SetActorScale3D(NewScale);						// Sets duplicate actor to scale vector
		TArray<UPrimitiveComponent*> OverlapActors;
		DuplicateMeshActor->GetOverlappingComponents(OverlapActors);
		if (OverlapActors.Num() != 0) {
			UE_LOG(LogTemp, Error, TEXT("You got yourself a big boi"));
		}

		proceed = false;	// Makes it so Select() cannot be ran until no longer a trigger time
	}
}