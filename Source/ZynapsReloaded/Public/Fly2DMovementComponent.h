// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Components/ActorComponent.h"
#include "Fly2DMovementComponent.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogFly2DMovementComponent, Log, All);

// This constants applied to avoid artifacts when the player is near the screen bounds
const float LimitMarginUp = 0.0f;
const float LimitMarginDown = 0.0f;
const float LimitMarginLeft = 50.0f;
const float LimitMarginRight = 50.0f;

/**
 * Component to manage the movement of a flying machine in a 2.5D game.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZYNAPSRELOADED_API UFly2DMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFly2DMovementComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaSeconds, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;

	// Called to move the actor up
	UFUNCTION(BlueprintCallable, Category = Actions)
	void MoveUp(float Val);

	// Called to move the actor down
	UFUNCTION(BlueprintCallable, Category = Actions)
	void MoveDown(float Val);

	// Called to move the actor to the left
	UFUNCTION(BlueprintCallable, Category = Actions)
	void MoveLeft(float Val);

	// Called to move the actor to the right
	UFUNCTION(BlueprintCallable, Category = Actions)
	void MoveRight(float Val);

	// Called to stop the actor's movement
	UFUNCTION(BlueprintCallable, Category = Actions)
	void StopMovement();

	// Updated component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	USceneComponent* UpdatedComponent;

	// The actor's initial movement speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float InitialMovementSpeed;

	// The actor's initial acceleration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float InitialAcceleration;

	// The actor's speed and acceleration increase (0 - 3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	uint8 SpeedUpLevel;

	// Maximum rotation when moving up or down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rotation)
	float MaxRotation;

	// Rotation speed when moving up or down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rotation)
	float RotationSpeed;

	// Rotation recovery speed when the actor is not moving up or down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rotation)
	float RotationRecoverySpeed;

private:

	// Called from TickComponent() to calculate and apply movement to the updated component based on user input
	void ApplyActorMovement(float DeltaSeconds);

	// Calculates and applies rotation to the updated component when moving up and down
	void ApplyActorRotation(USceneComponent* ComponentToUpdate, float RotationToApply, float DeltaSeconds);

	// Returns the owner component to update. If an updated component was not set, returns the root component
	// of the owner.
	USceneComponent* GetSafeUpdatedComponent() const;

	// Flag to indicate that the actor should move up
	bool bMoveUp;

	// Flag to indicate that the actor should move down
	bool bMoveDown;

	// Flag to indicate that the actor should move left
	bool bMoveLeft;

	// Flag to indicate that the actor should move right
	bool bMoveRight;

	// The actor's current speed
	FVector2D CurrentSpeed;

	// The actor's current rotation
	float CurrentRotation;
};
