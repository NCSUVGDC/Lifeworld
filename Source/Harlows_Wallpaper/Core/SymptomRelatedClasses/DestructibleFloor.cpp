#include "DestructibleFloor.h"

// Initialization
ADestructibleFloor::ADestructibleFloor()
{
	//Actor doesn't need to tick
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));

	//Box from floor to ceiling to ascertain objects in that area
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent Component"));
	BoxComponent->SetupAttachment(RootComponent);

	//The Destructible mesh for the floor, removed at end of symptom
	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Destructible Component"));
	DestructibleComponent->SetupAttachment(RootComponent);

	//Static mesh that will replace dectructible mesh
	Failsafe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	Failsafe->SetupAttachment(RootComponent);

	//For now, Failsafe should only block the player
	Failsafe->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Failsafe->SetVisibility(false);

	bCanBeDamaged = false;

}

void ADestructibleFloor::StartSymptom(float StartRestoreTime)
{

	float Delay = FMath::RandRange(4.f, 10.f);

	//Set timer for instigating falling floor
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADestructibleFloor::FallToRuin, Delay);

	//Set timer for start of recovery checks
	GetWorldTimerManager().SetTimer(TimerHandle1, this, &ADestructibleFloor::StartChecking, StartRestoreTime);
	
}

void ADestructibleFloor::FallToRuin()
{

	//Remember initial Position before being destroyed
	Position = DestructibleComponent->GetComponentLocation();
	bCanBeDamaged = true;

	//Get bounds of floor up to ceiling
	FBox FloorVolume = BoxComponent->Bounds.GetBox();

	UE_LOG(LogTemp, Warning, TEXT("Fall %s\n"), *this->GetName());

	//Find all actors that should fall
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TagName, FoundActors);

	for (AActor* Current : FoundActors) {
		
		//Get bounding box of actor
		FBox ActorBox = Current->GetComponentsBoundingBox();
		
		//Check if they are in the floor's bounds
		if (FloorVolume.Intersect(ActorBox)) {
			//Map their name to their transform and store it
			Map.Add(Current->GetName(), Current->GetTransform());
			UE_LOG(LogTemp, Warning, TEXT("%s is on %s\n"), *Current->GetName(), *this->GetName());
		}
	}

	//Destruct mesh
	DestructibleComponent->ApplyDamage(10000, Position, FVector(0, 0, -10), 100);

}

void ADestructibleFloor::StartChecking()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s should start checking\n"), *this->GetName());
	float f = 1 / 60.0;

	//Remove mesh now that's been used
	DestructibleComponent->DestroyComponent();

	// Execute check for recovery at set intervals
	GetWorldTimerManager().SetTimer(TimerHandle2, this, &ADestructibleFloor::Check, f, true, 0);
	ShouldCheck = true;
}

void ADestructibleFloor::Restore()
{
	//UE_LOG(LogTemp, Warning, TEXT("Restore %s\n"), *this->GetName());

	ShouldCheck = false;

	//Set up Failsafe to be the new floor
	Failsafe->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Failsafe->SetVisibility(true);
	
	//Get all actors that should've fallen
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TagName, FoundActors);

	//Sort the Map to ensure objects are ordered by z-value (Ensures objects don't get reset before the object that supports them)
	Map.ValueSort([](const FTransform& A, const FTransform& B) {
		return A.GetLocation().Z < B.GetLocation().Z; // sort transforms by z height
	});

	//Find stored actors and reset them
	for (TPair<FString, FTransform>& Current : Map) {
		//Iterate through actors to find correct one
		for (AActor* F : FoundActors) {
			if (Current.Key == F->GetName()) {
				//Reset actor to stored Position if it hasn't already been
				if (!F->GetActorTransform().Equals(Current.Value)) {
					//F->SetActorTransform(Current.Value);
					F-> SetActorLocationAndRotation(Current.Value.GetLocation(), Current.Value.GetRotation(), false, nullptr, ETeleportType::ResetPhysics);
					UE_LOG(LogTemp, Warning, TEXT("%s Reset %s\n"), *this->GetName(), *Current.Key);
				}
				break;
			}
		}
	}

}

// Called when the game starts or when spawned
void ADestructibleFloor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ADestructibleFloor::Tick(float DeltaTime)
{
}

// Called at set interval during symptom recovery time
void ADestructibleFloor::Check()
{
	//If already recovered
	if (!ShouldCheck) {
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s is still looking"), *this->GetName());


	//Check if player is looking at actor
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	FVector Forward = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();

	FVector Loc = Player->GetActorLocation();

	if (BoxComponent->Bounds.GetBox().IsInsideOrOn(Loc)){
		Failsafe->SetVisibility(true);
		return;
	}
	else {
		Failsafe->SetVisibility(false);
	}

	float F = FVector::DotProduct(Position - Loc, Forward);

	//If player can't see actor
	if (F < .57) {
		Restore();
	}
}

