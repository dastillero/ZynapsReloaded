// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "PlayerPawn.h"

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

	// Set up the Projector 2D component
	Projector2DComponent = CreateProjector2DComponent();

	// Sets this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Init movement vars
	InitialMovementSpeed = 60.0f;
	InitialAcceleration = 150.0f;
	CurrentSpeed = FVector2D(0.0f, 0.0f);
	bMoveUp = false;
	bMoveDown = false;
	bMoveLeft = false;
	bMoveRight = false;

	// Inits player state
	AZynapsPlayerState* State = GetZynapsPlayerState();
	if (State)
	{
		State->GameScore = 0;
		State->Lives = 3;
		State->SpeedUpLevel = 0;
		State->LaserPower = 0;
		State->PlasmaBombs = false;
		State->HomingMissiles = false;
		State->SeekerMissiles = false;
	}

	// Init shooting vars
	ProjectileClass = APlayerProjectile::StaticClass();
	LeftCannonSocketName = FName("LeftCannon");
	RightCannonSocketName = FName("RightCannon");
	TopCannonSocketName = FName("TopCannon");
	NextCannon = RightCannon;

	// Init rotation vars
	MaxRotation = 37.5f;
	RotationSpeed = 250.0f;
	RotationRecoverySpeed = 200.0f;
	CurrentRotation = 0.0f;
}

// Creates the capsule component used for collision detection
UCapsuleComponent* APlayerPawn::CreateCapsuleComponent(USceneComponent* Parent)
{
	UCapsuleComponent* Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	Component->InitCapsuleSize(18.0f, 32.0f);
	Component->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
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
	Component->SetSimulatePhysics(true);
	Component->SetEnableGravity(false);
	Component->SetAngularDamping(0.0f);
	Component->SetLinearDamping(0.0f);
	//Component->SetConstraintMode(EDOFMode::YZPlane);
	Component->BodyInstance.bLockXRotation = true;
	//Component->BodyInstance.bLockYRotation = true;
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

// Creates the Projector 2D component
UProjector2DComponent* APlayerPawn::CreateProjector2DComponent()
{
	UProjector2DComponent* Component = CreateDefaultSubobject<UProjector2DComponent>(TEXT("Projector2DComponent"));
	return Component;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Apply the camera offset to the player. This should be done by means of a proper 
	// PlayerCameraManager
	CapsuleComponent->AddWorldOffset(FVector(0.0f, 1000.0f * DeltaSeconds, 0.0f));

	// Apply player movement
	ApplyPlayerMovement(DeltaSeconds);
}

// Called from Tick() to calculate and apply movement to the player based on user input
void APlayerPawn::ApplyPlayerMovement(float DeltaSeconds)
{
	// Get the values needed to calculate the player's movement
	float MaxMovementSpeed = InitialMovementSpeed;
	float MaxAcceleration = InitialAcceleration;
	AZynapsPlayerState* State = GetZynapsPlayerState();
	if (State)
	{
		MaxMovementSpeed += ((float)State->SpeedUpLevel) / 10.0f;
		MaxAcceleration += ((float)State->SpeedUpLevel) / 10.0f;
	}

	// Init rotation
	float RotationToApply = 0.0f;

	// Calculate the new vertical speed
	if (bMoveUp && !bMoveDown)
	{
		// Accelerate up
		CurrentSpeed.Y += MaxAcceleration * DeltaSeconds;
		if (CurrentSpeed.Y > MaxMovementSpeed) CurrentSpeed.Y = MaxMovementSpeed;

		// Set rotation to apply
		RotationToApply = RotationSpeed;
	}
	else if (bMoveDown && !bMoveUp)
	{
		// Accelerate down
		CurrentSpeed.Y -= MaxAcceleration * DeltaSeconds;
		if (CurrentSpeed.Y < -MaxMovementSpeed) CurrentSpeed.Y = -MaxMovementSpeed;

		// Set rotation to apply
		RotationToApply = -RotationSpeed;
	}
	else
	{
		// Decelerate in the vertical axis
		if (CurrentSpeed.Y != 0.0f)
		{
			if (CurrentSpeed.Y > 0.0f)
			{
				CurrentSpeed.Y -= MaxAcceleration * DeltaSeconds;
				if (CurrentSpeed.Y < 0.0f) CurrentSpeed.Y = 0.0f;
			}
			else
			{
				CurrentSpeed.Y += MaxAcceleration * DeltaSeconds;
				if (CurrentSpeed.Y > 0.0f) CurrentSpeed.Y = 0.0f;
			}
		}
	}

	// Calculate the new horizontal speed
	if (bMoveLeft && !bMoveRight)
	{
		// Accelerate left
		CurrentSpeed.X -= MaxAcceleration * DeltaSeconds;
		if (CurrentSpeed.X < -MaxMovementSpeed) CurrentSpeed.X = -MaxMovementSpeed;
	}
	else if (bMoveRight && !bMoveLeft)
	{
		// Accelerate right
		CurrentSpeed.X += MaxAcceleration * DeltaSeconds;
		if (CurrentSpeed.X > MaxMovementSpeed) CurrentSpeed.X = MaxMovementSpeed;
	}
	else
	{
		// Decelerate in the horizontal axis
		if (CurrentSpeed.X != 0.0f)
		{
			if (CurrentSpeed.X > 0.0f)
			{
				CurrentSpeed.X -= MaxAcceleration * DeltaSeconds;
				if (CurrentSpeed.X < 0.0f) CurrentSpeed.X = 0.0f;
			}
			else
			{
				CurrentSpeed.X += MaxAcceleration * DeltaSeconds;
				if (CurrentSpeed.X > 0.0f) CurrentSpeed.X = 0.0f;
			}
		}
	}

	// Calculate the next position to occupy
	FVector NextLocation = CapsuleComponent->GetComponentLocation();
	NextLocation.Z += CurrentSpeed.Y;
	NextLocation.Y += CurrentSpeed.X;

	// Move the player to the next position if it is within screen bounds. The viewing distance should be get
	// from the camera itself instead of using a magic number
	FVector2D ViewportSize = Projector2DComponent->GetViewportSize();
	FVector PlayerExtent = CapsuleComponent->Bounds.BoxExtent;
	FVector TopLeftBound = Projector2DComponent->ConvertFromScreenCoordinates(FVector2D::ZeroVector, 20000);
	FVector BottomRightBound = Projector2DComponent->ConvertFromScreenCoordinates(ViewportSize, 20000);
	float MaxZ = TopLeftBound.Z - PlayerExtent.Z - LimitMarginUp;
	float MinZ = BottomRightBound.Z + PlayerExtent.Z + LimitMarginDown;
	float MinY = TopLeftBound.Y + PlayerExtent.Y + LimitMarginLeft;
	float MaxY = BottomRightBound.Y - PlayerExtent.Y - LimitMarginRight;
	NextLocation.Z = FMath::Clamp<float>(NextLocation.Z, MinZ, MaxZ);
	NextLocation.Y = FMath::Clamp<float>(NextLocation.Y, MinY, MaxY);
	ApplyPlayerRotation(RotationToApply, DeltaSeconds);

	// Vertical axis
	if (NextLocation.Z < MaxZ && NextLocation.Z > MinZ)
	{
		CapsuleComponent->SetWorldLocation(NextLocation);
	}
	else
	{
		CurrentSpeed.Y = 0.0f;
	}

	// Horizontal axis
	if (NextLocation.Y > MinY && NextLocation.Y < MaxY)
	{
		CapsuleComponent->SetWorldLocation(NextLocation);
	}
	else
	{
		CurrentSpeed.X = 0.0f;
	}

	// Clear movement flags
	bMoveUp = bMoveDown = bMoveLeft = bMoveRight = false;
}

// Calculates and applies rotation to the player when moving up and down
void APlayerPawn::ApplyPlayerRotation(float RotationToApply, float DeltaSeconds)
{
	if (RotationToApply != 0.0f)
	{
		// There is a rotation to apply (up or down buttons are pressed)
		CurrentRotation += RotationToApply * DeltaSeconds;
		if (RotationToApply > 0.0f)
		{
			// Positive rotation
			if (CurrentRotation > MaxRotation)
			{
				CurrentRotation = MaxRotation;
			}
		}
		else
		{
			// Negative rotation
			if (CurrentRotation < -MaxRotation)
			{
				CurrentRotation = -MaxRotation;
			}
		}
		RotationToApply = 0.0f;
	}
	else
	{
		// There is no rotation to apply (up and down buttons are released)
		if (CurrentRotation != 0.0f)
		{
			// The player is rotated
			if (CurrentRotation > 0.0f)
			{
				// Positive rotation
				CurrentRotation -= RotationRecoverySpeed * DeltaSeconds;
				if (CurrentRotation < 0.0f)
				{
					CurrentRotation = 0.0f;
				}
			}
			else
			{
				// Negative rotation
				CurrentRotation += RotationRecoverySpeed * DeltaSeconds;
				if (CurrentRotation > 0.0f)
				{
					CurrentRotation = 0.0f;
				}
			}
		}
	}
	CapsuleComponent->SetRelativeRotation(FRotator(CurrentRotation, 180.0f, -90.0f));
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

// Called to move the player up
void APlayerPawn::MoveUp(float Val)
{
	bMoveUp = true;
}

// Called to move the player down
void APlayerPawn::MoveDown(float Val)
{
	bMoveDown = true;
}

// Called to move the player to the left
void APlayerPawn::MoveLeft(float Val)
{
	bMoveLeft = true;
}

// Called to move the player to the right
void APlayerPawn::MoveRight(float Val)
{
	bMoveRight = true;
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

// Called when the player hits other actor
void APlayerPawn::Hit_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &HitResult)
{
	//PRINT("APlayerPawn::Hit");

	// Ensure the player does not change angle or direction when hits something
	CapsuleComponent->BodyInstance.SetAngularVelocity(FVector(0.0f, 0.0f, 0.0f), false);
	CapsuleComponent->BodyInstance.SetLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false);
	CapsuleComponent->SetWorldRotation(FRotator(0.0f, 180.0f, -90.0f));
}

// Called when the player begins overlapping with another actor
void APlayerPawn::BeginOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult &SweepResult)
{
	// PRINT("APlayerPawn::BeginOverlap");
}

// Called when the player ends overlapping with another actor
void APlayerPawn::EndOverlap_Implementation(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// PRINT("APlayerPawn::EndOverlap");
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
