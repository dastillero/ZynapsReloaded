// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerState.h"
#include "ZynapsPlayerState.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsPlayerState, Log, All);

/**
 * Enumeration storing the indexes of the power-ups.
 */
UENUM(BlueprintType)
enum class EPowerUp : uint8
{
	SpeedUp = 0,
	LaserPower = 1,
	PlasmaBombs = 2,
	HomingMissiles = 3,
	SeekerMissiles = 4
};

/**
 * Enumeration storing the states of the player during gameplay.
 */
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Playing = 0,
	Destroyed = 1
};

/**
 * This class stores the player's state information.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	// Default constructor
	AZynapsPlayerState();

	// Returns the current player state
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	EPlayerState GetCurrentState() const;

	// Sets the current player state
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetCurrentState(EPlayerState State);

	// Returns the power-up activation mode
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	bool GetPowerUpActivationMode() const;

	// Sets the power-up activation mode
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetPowerUpActivationMode(bool NewPowerUpActivationMode);

	// Score
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	int32 GameScore;

	// Remaining lives
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	uint8 Lives;

	// Speed up level (0 - 3)
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	uint8 SpeedUpLevel;

	// Laser power (0 - 3)
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	uint8 LaserPower;

	// Plasma bombs
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	bool PlasmaBombs;

	// Homing missiles
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	bool HomingMissiles;

	// Seeker missiles
	UPROPERTY(Replicated, BlueprintReadWrite, Category = ZynapsState)
	bool SeekerMissiles;

	// Returns the selected power-up
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	EPowerUp GetSelectedPowerUp() const;

	// Called when a fuel capsule is collected
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void FuelCapsuleCollected();

protected:

	// Reduces a live and resets the state
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void ReduceLives();

	// Cycles through the power-ups
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void ShiftSelectedPowerUp();

	// Activates the selected power-up
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void ActivateSelectedPowerUp();

private:

	// The player's current state
	EPlayerState CurrentState;

	// Power-up to be activated
	EPowerUp PowerUp;

	// Flag which indicates that the player is in power-up activation mode
	bool PowerUpActivationMode;
};
