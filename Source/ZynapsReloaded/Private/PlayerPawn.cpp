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
	//Component->SetNotifyRigidBodyCollision(true);
	Component->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Component->SetCanEverAffectNavigation(false);
	Component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Component->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	Component->SetSimulatePhysics(false);
	Component->SetEnableGravity(false);
	Component->SetAngularDamping(0.0f);
	Component->SetLinearDamping(0.0f);
	Component->BodyInstance.bLockXRotation = true;
	Component->BodyInstance.bLockZRotation = true;
	Component->BodyInstance.bLockXTranslation = true;
	//Component->SetHiddenInGame(false);
	//Component->WakeRigidBody();
	Component->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::BeginOverlap);
	Component->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::EndOverlap);
	//Component->OnComponentHit.AddDynamic(this, &APlayerPawn::Hit);
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

// Called after component initialization
void APlayerPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Convert the first material of the mesh in a dynamic instance in order to change its color
	// during the power-up activation mode
	DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (!DynMaterial)
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("The dynamic material instance could not be created"));
		return;
	}
	DynMaterial->SetScalarParameterValue(FName("HighlightAlpha"), 0.0f);
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

	// If the game is in Preparing state, ensure that the pawn is located exactly at the player start. This is 
	// really a fix as the game mode spawn functions doesn't seem to work properly the first time a pawn is 
	// spawned at a player start.
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		if (Controller)
		{
			if (Controller->StartSpot != nullptr)
			{
				CapsuleComponent->SetWorldLocation(Controller->StartSpot.Get()->GetActorLocation());
			}
		}
		return;
	}

	// Apply the scroll speed to the player
	AZynapsWorldSettings* WorldSettings = AZynapsWorldSettings::GetZynapsWorldSettings(GetWorld());
	if (WorldSettings)
	{
		CapsuleComponent->AddWorldOffset(FVector(0.0f, WorldSettings->ScrollSpeed * DeltaSeconds, 0.0f));
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("Failed to retrieve the stage world settings"));
	}

	// Get the player state
	AZynapsPlayerState* ZynapsPlayerState = GetZynapsPlayerState();
	if (!ZynapsPlayerState)
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("Failed to retrieve the player state"));
		return;
	}

	// If the player is in power-up activation mode, perform the corresponding effect
	FName ParamName("HighlightGlow");
	if (ZynapsPlayerState->GetPowerUpActivationMode())
	{
		float GlowValue;
		DynMaterial->GetScalarParameterValue(ParamName, GlowValue);
		GlowValue += HighlightDirection * HighlightGlowSpeed * DeltaSeconds;
		if (GlowValue <= 0.0f || GlowValue >= 0.5f)
		{
			HighlightDirection *= -1.0f;
		}
		DynMaterial->SetScalarParameterValue(ParamName, GlowValue);
	}
	else
	{
		DynMaterial->SetScalarParameterValue(ParamName, 0.0f);
		HighlightDirection = 1.0f;
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

	// Shot the cannon and play the corresponding sound
	GetWorld()->SpawnActor<APlayerProjectile>(ProjectileClass, CannonTransforms[NextCannon]);
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("No sound specified when firing a cannon"));
	}

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
}

// Called when the player begins overlapping with another actor
void APlayerPawn::BeginOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult &SweepResult)
{
	AZynapsPlayerState* ZynapsPlayerState = GetZynapsPlayerState();
	if (!ZynapsPlayerState)
	{
		UE_LOG(LogPlayerPawn, Error, TEXT("Failed to retrieve the player state"));
		return;
	}

	if (OtherActor->IsA(AFuelCapsule::StaticClass()))
	{
		// Overlapping a fuel capsule
		FuelCapsuleCollected(ZynapsPlayerState, Cast<AFuelCapsule>(OtherActor));
	}
	else
	{
		// Overlapping other actor
		PlayerPawnDestroyed(ZynapsPlayerState);
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

// Called when a fuel capsule is collected
void APlayerPawn::FuelCapsuleCollected(AZynapsPlayerState* ZynapsPlayerState, AFuelCapsule* FuelCapsule)
{
	if (ZynapsPlayerState->GetPowerUpActivationMode())
	{
		ZynapsPlayerState->ActivateSelectedPowerUp();
		if (ActivatePowerUpSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ActivatePowerUpSound);
		}
		else
		{
			UE_LOG(LogPlayerPawn, Warning, TEXT("No sound specified for power-up activation"));
		}
	}
	else
	{
		ZynapsPlayerState->ShiftSelectedPowerUp();
		if (ShiftPowerUpSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ShiftPowerUpSound);
		}
		else
		{
			UE_LOG(LogPlayerPawn, Warning, TEXT("No sound specified for power-up shifting"));
		}
	}
	FuelCapsule->Destroy();
}

// Called when the player pawn is destroyed
void APlayerPawn::PlayerPawnDestroyed(AZynapsPlayerState* ZynapsPlayerState)
{
	// Spawn an explosion at the actor's location
	if (ExplosionPartSystem)
	{
		FTransform Transform(FRotator(0.0f, 0.0f, 0.0f), CapsuleComponent->GetComponentLocation(),
			FVector(7.5f, 7.5f, 7.5f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionPartSystem, Transform, true);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("No particle system specified for player explosion"));
	}

	// Play a sound
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
	}
	else
	{
		UE_LOG(LogPlayerPawn, Warning, TEXT("No sound specied for player explosion"));
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
	ZynapsPlayerState->SetCurrentState(EPlayerState::Destroyed);
	Destroy();
}
