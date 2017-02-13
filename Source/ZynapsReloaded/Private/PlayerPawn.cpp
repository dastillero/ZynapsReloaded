// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "PlayerPawn.h"
#include "ProjectionUtil.h"
#include "ZynapsWorldSettings.h"
#include "FuelCapsule.h"

// Log category
DEFINE_LOG_CATEGORY(LogPlayerPawn);

// Sets default values
APlayerPawn::APlayerPawn() : Super()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Set up the collision capsule
	CapsuleComponent = CreateCapsuleComponent(RootComponent);

	// Set up the mesh component
	MeshComponent = CreateMeshComponent(CapsuleComponent);

	// Set up the engine thrust particle system
	EngineThrustSocketName = TEXT("EngineThrust");
	EnginePartSystemComponent = CreateEngineThrustParticleSystem(MeshComponent, EngineThrustSocketName);

	// Set up the movement component
	MovementComponent = CreateMovementComponent();

	// Set up the explosion particle system
	ExplosionPartSystem = CreateExplosionParticleSystem();

	// Sets this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Init shooting vars
	ProjectileClass = APlayerProjectile::StaticClass();
	LeftCannonSocketName = FName("LeftCannon");
	RightCannonSocketName = FName("RightCannon");
	TopCannonSocketName = FName("TopCannon");
	NextCannon = RightCannon;
}

// Creates the capsule component used for collision detection
UCapsuleComponent* APlayerPawn::CreateCapsuleComponent(USceneComponent* Parent)
{
	UCapsuleComponent* Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Component->InitCapsuleSize(18.0f, 32.0f);
	Component->SetWorldRotation(FRotator(0.0f, 180.0f, -90.0f));
	Component->SetWorldScale3D(FVector(12.0f, 10.0f, 10.0f));
	Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Component->bGenerateOverlapEvents = true;
	Component->SetNotifyRigidBodyCollision(true);
	Component->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Component->SetCanEverAffectNavigation(false);
	Component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	Component->SetSimulatePhysics(true);
	Component->SetEnableGravity(false);
	Component->SetAngularDamping(0.0f);
	Component->SetLinearDamping(0.0f);
	Component->BodyInstance.bLockXRotation = true;
	Component->BodyInstance.bLockZRotation = true;
	Component->BodyInstance.bLockXTranslation = true;
	//Component->SetHiddenInGame(false);
	Component->WakeRigidBody();
	Component->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::BeginOverlap);
	Component->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::EndOverlap);
	Component->OnComponentHit.AddDynamic(this, &APlayerPawn::Hit);
	Component->SetupAttachment(Parent);

	return Component;
}

// Creates the mesh component which models the ship
UStaticMeshComponent* APlayerPawn::CreateMeshComponent(USceneComponent* Parent)
{
	UStaticMeshComponent* Component = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(
		TEXT("StaticMesh'/Game/Models/Ship/zynaps_ship.zynaps_ship'"));
	if (MeshObj.Succeeded())
	{
		Component->SetStaticMesh(MeshObj.Object);
		Component->SetRelativeLocation(FVector(0.0f, -3.5f, -7.0f));
		Component->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		Component->SetRelativeScale3D(FVector(1.0f / 12.0f, 0.1f, 0.1f));
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetSimulatePhysics(false);
		Component->SetupAttachment(Parent);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Error, 
			TEXT("The asset StaticMesh'/Game/Models/Ship/zynaps_ship.zynaps_ship' was not found"));
		Component = nullptr;
	}

	return Component;
}

// Creates the particle system for the engine thrust
UParticleSystemComponent* APlayerPawn::CreateEngineThrustParticleSystem(USceneComponent* Parent, FName SocketName)
{
	UParticleSystemComponent* Component = 
		CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EnginePartSystemComponent"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PartSystem(
		TEXT("ParticleSystem'/Game/Models/ThrustParticle/EngineThrustSystem.EngineThrustSystem'"));
	if (PartSystem.Succeeded())
	{
		Component->SetTemplate(PartSystem.Object);
		Component->SetRelativeScale3D(FVector(1.5f, 0.5f, 1.5f));
		Component->bAutoActivate = true;
		Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Component->SetSimulatePhysics(false);
		Component->SetupAttachment(Parent, SocketName);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Error,
			TEXT("The asset ParticleSystem'/Game/Models/ThrustParticle/EngineThrustSystem.EngineThrustSystem' was not found"));
		Component = nullptr;
	}

	return Component;
}

// Creates the component which manages the movement of the ship
UFly2DMovementComponent* APlayerPawn::CreateMovementComponent()
{
	UFly2DMovementComponent* Component = CreateDefaultSubobject<UFly2DMovementComponent>(TEXT("MovementComponent"));
	Component->UpdatedComponent = CapsuleComponent;
	return Component;
}

// Creates the explosion particle system
UParticleSystem* APlayerPawn::CreateExplosionParticleSystem()
{
	UParticleSystem* Result;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PartSystem(
		TEXT("ParticleSystem'/Game/Models/Explosion/ExplosionSystem.ExplosionSystem'"));
	if (PartSystem.Succeeded())
	{
		Result = PartSystem.Object;
	}
	else
	{
		UE_LOG(LogPlayerPawn, Error,
			TEXT("The asset ParticleSystem'/Game/Models/Explosion/ExplosionSystem.ExplosionSystem' was not found"));
		Result = nullptr;
	}

	return Result;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Init the player state
	AZynapsPlayerState* State = GetZynapsPlayerState();
	if (!State)
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("Failed to retrieve the player state"));
		return;
	}
	State->SetCurrentState(EPlayerState::Playing);
}

// Called every frame
void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is not in Preparing state, apply the scroll speed to the player
	if (ZynapsGameState->GetCurrentState() != EStageState::Preparing)
	{
		AZynapsWorldSettings* WorldSettings = AZynapsWorldSettings::GetZynapsWorldSettings(GetWorld());
		if (WorldSettings)
		{
			CapsuleComponent->AddWorldOffset(FVector(0.0f, WorldSettings->ScrollSpeed * DeltaSeconds, 0.0f));
		}
		else
		{
			UE_LOG(LogPlayerPawn, Warning, TEXT("Failed to retrieve the stage world settings"));
		}
	}
	else
	{
		// This is really a fix as the game mode spawn functions doesn't seem to work properly the
		// first time a pawn is spawned at a player start. This code ensures that the pawn is
		// located exactly at the player start
		if (Controller)
		{
			if (Controller->StartSpot != nullptr)
			{
				CapsuleComponent->SetWorldLocation(Controller->StartSpot.Get()->GetActorLocation());
			}
		}
	}
}

// Returns a reference to the instance of AZynapsPlayerState or NULL if it doesn't exist
AZynapsPlayerState* APlayerPawn::GetZynapsPlayerState() const
{
	AZynapsPlayerState* Result = NULL;
	if (PlayerState)
	{
		Result = Cast<AZynapsPlayerState>(PlayerState);
	}
	return Result;
}

// Returns the game state
AZynapsGameState* APlayerPawn::GetZynapsGameState() const
{
	return GetWorld()->GetGameState<AZynapsGameState>();
}

// Called to move the player up
void APlayerPawn::MoveUp(float Val)
{
	MovementComponent->MoveUp(Val);
}

// Called to move the player down
void APlayerPawn::MoveDown(float Val)
{
	MovementComponent->MoveDown(Val);
}

// Called to move the player to the left
void APlayerPawn::MoveLeft(float Val)
{
	MovementComponent->MoveLeft(Val);
}

// Called to move the player to the right
void APlayerPawn::MoveRight(float Val)
{
	MovementComponent->MoveRight(Val);
}

// Called to fire
void APlayerPawn::Fire()
{
	// Get the transforms for the cannons
	FTransform CannonTransforms[3] = {
		GetSocketTransform(RightCannonSocketName),
		GetSocketTransform(LeftCannonSocketName),
		GetSocketTransform(TopCannonSocketName)
	};

	// Shot the cannon
	GetWorld()->SpawnActor<APlayerProjectile>(ProjectileClass, CannonTransforms[NextCannon]);

	// Prepare the next cannon to be shot
	if (++NextCannon > TopCannon)
	{
		NextCannon = RightCannon;
	}
}

// Called when the player hits other actor or an obstacle
void APlayerPawn::Hit_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &HitResult)
{
	// Spawn an explosion at the actor's location
	if (ExplosionPartSystem)
	{
		FTransform Transform(FRotator(0.0f, 0.0f, 0.0f), CapsuleComponent->GetComponentLocation(),
			FVector(7.5f, 7.5f, 7.5f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionPartSystem, Transform, true);
	}

	// Play the camera shake
	if (Controller)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		if (CameraShakeClass)
		{
			PlayerController->ClientPlayCameraShake(CameraShakeClass);
		}
		else
		{
			UE_LOG(LogPlayerPawn, Warning, TEXT("The camera shake class is not set"));
		}
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("Failed to retrieve the player controller"));
	}

	// Update the player state and destroy the actor
	AZynapsPlayerState* State = GetZynapsPlayerState();
	if (State)
	{
		State->SetCurrentState(EPlayerState::Destroyed);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("Failed to retrieve the player state for update"));
	}
	Destroy();
}

// Called when the player begins overlapping with another actor
void APlayerPawn::BeginOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AFuelCapsule::StaticClass()))
	{
		// Overlapping a fuel capsule
		AZynapsPlayerState* ZynapsPlayerState = GetZynapsPlayerState();
		if (!ZynapsPlayerState)
		{
			UE_LOG(LogPlayerPawn, Warning, TEXT("Failed to retrieve the player state"));
		}
		else
		{
			ZynapsPlayerState->FuelCapsuleCollected();
		}
		OtherActor->Destroy();
	}
}

// Called when the player ends overlapping with another actor
void APlayerPawn::EndOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Returns the transform of a socket
FTransform APlayerPawn::GetSocketTransform(FName SocketName) const
{
	FTransform Result;
	if (MeshComponent->DoesSocketExist(SocketName))
	{
		Result = MeshComponent->GetSocketTransform(SocketName);
	}
	return Result;
}
