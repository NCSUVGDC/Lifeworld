// Fill out your copyright notice in the Description page of Project Settings.

#include "HarlowPawn.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"


// Sets default values
AHarlowPawn::AHarlowPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHarlowPawn::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Beginning play"));

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
					DoubleTakeActors.Add(SymptomActor);
				}
			}
		}
	}

	//Get reference to phantom
	//Find the phantom that exists in the world and store a reference to it
	TArray<AActor*> GhostsNStuff;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);
	//	FString NumStr = FString::FromInt(GhostsNStuff.Num());
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *NumStr);

	//If we have the one phantom found, store the reference at index 0 of TArray
	if (GhostsNStuff.Num() == 1)
	{

		//Call the function in 5 seconds
	/*	if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Gonna impose phantom"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Gonna impose phantom"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Gonna impose phantom"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Gonna impose phantom"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Gonna impose phantom"));
		}*/
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 5.0f, false);
	}
	else
	{
		//Call the function in 5 seconds
	/*	if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Gonna impose double take"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Gonna impose double take"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Gonna impose double take"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Gonna impose double take"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Gonna impose double take"));*/
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposeDoubleTake, 5.0f, false);
	}
	
}

// Called every frame
void AHarlowPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;

	FVector Start = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	Start.Z = 10;

	//	GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetRightVector();

	FVector RightVector = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransformComponent()->GetRightVector();
//	RightVector.Z = 10;
	FVector End = ((RightVector * 90.f) + Start);
	End.Z = 10;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

	//FVector RightVector = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransformComponent()->GetRightVector();
	//	RightVector.Z = 10;
	End = ((-RightVector * 90.f) + Start);
	End.Z = 10;


	DrawDebugLine(GetWorld(), Start, End, FColor::Turquoise, true);

/*	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if( isHit && OutHit.IsValidBlockingHit() )
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Turquoise, FString::Printf(TEXT("You are hitting %s"), *OutHit.GetActor()->GetName()));
	}
*/
}

// Called to bind functionality to input
void AHarlowPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHarlowPawn::ImposeDoubleTake()
{

	bool foundActor = false;
	AActor* closestObject = DoubleTakeActors[0];
	float closestDot = -1;
	// Add actors to symptom array
	for (AActor* object : DoubleTakeActors)
	{
		float dotProd = (GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager)->GetDotProductTo(object);
		if (dotProd >= 0.7 && dotProd < 0.8)
		{
			//Spawn a new PossessObject and tie player and object to it
			APossessedObject* pj = (APossessedObject*)GetWorld()->SpawnActor(APossessedObject::StaticClass());

			pj->setObject(object);
			pj->setPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			pj->DoASpoopyThing();
			foundActor = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Running Double-Take!"));
			break;
		}
		else if ( dotProd > closestDot && dotProd < 0.8 )
		{
			closestObject = object;
			closestDot = dotProd;
		}
	}

	if (!foundActor )
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

void AHarlowPawn::ImposePhantom()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Running Phantom!"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Running Phantom!"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Running Phantom!"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Running Phantom!"));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Running Phantom!"));
	}
	FHitResult OutHit;
	FVector Start = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	Start.Z = 17;
	//	GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetRightVector();
	FVector RightVector = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetTransformComponent()->GetRightVector();
	//	RightVector.Z = 10;
	FVector End = ((RightVector * 90.f) + Start);
	End.Z = 17;
	FCollisionQueryParams CollisionParams;

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

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
			phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			FVector ghostLoc = ((RightVector * 80.f) + Start);
			ghostLoc.Z = 0;
			phantom->SetActorLocation(ghostLoc);
			phantom->SetActorTickEnabled(true);
			FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(phantom->GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
			facePlayer.Roll = 0;
			facePlayer.Pitch = 0;
			facePlayer.Yaw -= 90;
			phantom->SetActorRotation(facePlayer);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 15.0f, false);
		}
	}
	FVector LeftVector = -RightVector;
	End = ((LeftVector * 90.f) + Start);
	End.Z = 17;

	isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

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
			phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
			FVector ghostLoc = ((LeftVector * 80.f) + Start);
			ghostLoc.Z = 0;
			phantom->SetActorLocation(ghostLoc);
			phantom->SetActorTickEnabled(true);
			FRotator facePlayer = UKismetMathLibrary::FindLookAtRotation(phantom->GetActorLocation(), GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation());
			facePlayer.Roll = 0;
			facePlayer.Pitch = 0;
			facePlayer.Yaw -= 90;
			phantom->SetActorRotation(facePlayer);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 15.0f, false);
		}
	}
	else
	{
	/*	if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TargetToClose. Let's try again!"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TargetToClose. Let's try again!"));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TargetToClose. Let's try again!"));
		}*/
		//Try again in 3 seconds
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 3.0f, false);
	}

/*
	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Turquoise, FString::Printf(TEXT("You are hitting something")));//, *OutHit.GetActor()->GetName()));

			AActor *actorHit = OutHit.GetActor();

			FVector actorHitRightVector = OutHit.ImpactPoint;

			//Get reference to phantom
			//Find the phantom that exists in the world and store a reference to it
			TArray<AActor*> GhostsNStuff;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);
			//	FString NumStr = FString::FromInt(GhostsNStuff.Num());
			//	if (GEngine)
			//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *NumStr);

			//If we have the one phantom found, store the reference at index 0 of TArray
			if (GhostsNStuff.Num() == 1)
			{
				phantom = (APhantom*)GhostsNStuff[0];
				phantom->SetActorHiddenInGame(false);
				phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
				phantom->SetActorLocation(actorHitRightVector);
				phantom->SetActorTickEnabled(true);
			}
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to run Phantom. Let's try again!"));
		//Try again in 3 seconds
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AHarlowPawn::ImposePhantom, 3.0f, false);
	}*/




	/*		if (GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Running Phantom"));

	//Get reference to phantom
	//Find the phantom that exists in the world and store a reference to it
	TArray<AActor*> GhostsNStuff;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhantom::StaticClass(), GhostsNStuff);
	//	FString NumStr = FString::FromInt(GhostsNStuff.Num());
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, *NumStr);

	//If we have the one phantom found, store the reference at index 0 of TArray
	if (GhostsNStuff.Num() == 1)
	{
	phantom = (APhantom*)GhostsNStuff[0];
	phantom->SetActorHiddenInGame(false);
	phantom->SetPlayer(GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager);
	FVector ghostLoc = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	ghostLoc += FVector(-50, 150, 0);
	phantom->SetActorLocation(ghostLoc);
	phantom->SetActorTickEnabled(true);
	}*/


}

