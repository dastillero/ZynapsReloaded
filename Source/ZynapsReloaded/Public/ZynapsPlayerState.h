// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerState.h"
#include "ZynapsPlayerState.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsPlayerState, Log, All);

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

	// Returns the index of the power-up to be activated
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	uint8 GetPowerUpIndex() const;

	// Called when a fuel capsule is collected
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void FuelCapsuleCollected();

protected:

	// Reduces a live and resets the state
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void ReduceLives();

	// Cycles through the power-up indexes
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void ShiftPowerUpIndex();

private:

	// The player's current state
	EPlayerState CurrentState;

	// Index of the power-up to be activated
	uint8 PowerUpIndex;

	// Flag which indicates that the player is in power-up activation mode
	bool PowerUpActivationMode;
};
