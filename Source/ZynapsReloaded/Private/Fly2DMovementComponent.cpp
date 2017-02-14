// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "Fly2DMovementComponent.h"
#include "ProjectionUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogFly2DMovementComponent);

// Sets default values for this component's properties
UFly2DMovementComponent::UFly2DMovementComponent() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Init action vars
	bMoveUp = false;
	bMoveDown = false;
	bMoveLeft = false;
	bMoveRight = false;

	// Init movement vars
	InitialMovementSpeed = 60.0f;
	InitialAcceleration = 150.0f;
	CurrentSpeed = FVector2D(0.0f, 0.0f);

	// Init rotation vars
	MaxRotation = 37.5f;
	RotationSpeed = 250.0f;
	RotationRecoverySpeed = 200.0f;
	CurrentRotation = 0.0f;
}

// Called when the game starts
void UFly2DMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UFly2DMovementComponent::TickComponent(float DeltaSeconds, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);

	// Apply actor movement
	ApplyActorMovement(DeltaSeconds);
}

// Called from TickComponent() to calculate and apply movement to the updated component based on user input
void UFly2DMovementComponent::ApplyActorMovement(float DeltaSeconds)
{
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogFly2DMovementComponent, Error, TEXT("Failed to retrieve the player controller"));
		return;
	}

	// Get the player state
	AZynapsPlayerState* ZynapsPlayerState = GetZynapsPlayerState();
	if (!ZynapsPlayerState)
	{
		UE_LOG(LogFly2DMovementComponent, Error, TEXT("Failed to retrieve the player state"));
		return;
	}

	// Get the component to update
	USceneComponent* ComponentToUpdate = GetSafeUpdatedComponent();

	// Get the values needed to calculate the actor's movement
	float SpeedUpLevel = (float)ZynapsPlayerState->GetSpeedUpLevel();
	float MaxMovementSpeed = InitialMovementSpeed +
		InitialMovementSpeed * (SpeedUpLevelIncrement * SpeedUpLevel);
	float MaxAcceleration = InitialAcceleration + 
		InitialAcceleration * (SpeedUpLevelIncrement * SpeedUpLevel);

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

	// Apply the actor rotation
	ApplyActorRotation(ComponentToUpdate, RotationToApply, DeltaSeconds);

	// Get the actor's origin and extent
	FVector ActorOrigin;
	FVector ActorExtent;
	GetOwner()->GetActorBounds(true, ActorOrigin, ActorExtent);

	// Calculate the viewport bounds
	FVector TopLeftBound;
	FVector BottomRightBound;
	if (!UProjectionUtil::CalculateViewportBounds(PlayerController, TopLeftBound, BottomRightBound))
	{
		UE_LOG(LogFly2DMovementComponent, Error, TEXT("Failed to calculate the viewport bounds"));
		return;
	}
	float MaxZ = TopLeftBound.Z - ActorExtent.Z - LimitMarginUp;
	float MinZ = BottomRightBound.Z + ActorExtent.Z + LimitMarginDown;
	float MinY = TopLeftBound.Y + ActorExtent.Y + LimitMarginLeft;
	float MaxY = BottomRightBound.Y - ActorExtent.Y - LimitMarginRight;

	// Calculate the next position to occupy
	FVector NextLocation = ComponentToUpdate->GetComponentLocation();
	NextLocation.Z += CurrentSpeed.Y;
	NextLocation.Y += CurrentSpeed.X;
	NextLocation.Z = FMath::Clamp(NextLocation.Z, MinZ, MaxZ);
	NextLocation.Y = FMath::Clamp(NextLocation.Y, MinY, MaxY);

	// Move the actor to the next position
	ComponentToUpdate->SetWorldLocation(NextLocation);

	// Reset speed to zero if the actor is touching the screen bounds
	if (NextLocation.Z >= MaxZ || NextLocation.Z <= MinZ)
	{
		CurrentSpeed.Y = 0.0f;
	}
	if (NextLocation.Y >= MaxY || NextLocation.Y <= MinY)
	{
		CurrentSpeed.X = 0.0f;
	}

	// Clear movement flags
	bMoveUp = bMoveDown = bMoveLeft = bMoveRight = false;
}

// Calculates and applies rotation to the updated component when moving up and down
void UFly2DMovementComponent::ApplyActorRotation(USceneComponent* ComponentToUpdate, float RotationToApply, 
	float DeltaSeconds)
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
	ComponentToUpdate->SetRelativeRotation(FRotator(CurrentRotation, 180.0f, -90.0f));
}

// Called to move the actor up
void UFly2DMovementComponent::MoveUp(float Val)
{
	bMoveUp = true;
}

// Called to move the actor down
void UFly2DMovementComponent::MoveDown(float Val)
{
	bMoveDown = true;
}

// Called to move the actor to the left
void UFly2DMovementComponent::MoveLeft(float Val)
{
	bMoveLeft = true;
}

// Called to move the actor to the right
void UFly2DMovementComponent::MoveRight(float Val)
{
	bMoveRight = true;
}

// Called to stop the actor's movement
void UFly2DMovementComponent::StopMovement()
{
	CurrentSpeed = FVector2D::ZeroVector;
}

// Returns the owner component to update. If an updated component was not set, returns the root component
// of the owner.
USceneComponent* UFly2DMovementComponent::GetSafeUpdatedComponent() const
{
	USceneComponent* Result = UpdatedComponent;
	if (!Result)
	{
		Result = GetOwner()->GetRootComponent();
	}
	return Result;
}

// Returns the player state
AZynapsPlayerState* UFly2DMovementComponent::GetZynapsPlayerState() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return nullptr;
	}

	if (!PlayerController->PlayerState)
	{
		return nullptr;
	}
	return Cast<AZynapsPlayerState>(PlayerController->PlayerState);
}
