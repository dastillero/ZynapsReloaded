// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerState.h"
#include "ZynapsPlayerState.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsPlayerState, Log, All);

// Number of initial lives
const uint8 InitialLives = 3;

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

	// Returns the selected power-up
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	EPowerUp GetSelectedPowerUp() const;

	// Cycles through the power-ups
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ShiftSelectedPowerUp();

	// Activates the selected power-up
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ActivateSelectedPowerUp();

	// Returns the game score
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	int32 GetGameScore() const;

	// Increases the game score
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void IncreaseGameScore(int32 Points);

	// Resets the game score
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ResetGameScore();

	// Returns the number of lives available
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	uint8 GetLives() const;

	// Increases a live
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void IncreaseLives();

	// Reduces a live and resets the power-up states
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void DecreaseLives();

	// Resets the number of lives
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ResetLives();

	// Returns the speed-up level
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	uint8 GetSpeedUpLevel() const;

	// Increases the speed-up level
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void IncreaseSpeedUpLevel();

	// Resets the speed-up level
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ResetSpeedUpLevel();

	// Returns the laser power level
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	uint8 GetLaserPower() const;

	// Increases the laser power level
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void IncreaseLaserPower();

	// Resets the laser power level
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void ResetLaserPower();

	// Returns the plasma bombs activation flag
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	bool GetPlasmaBombs() const;

	// Sets the value for the plasma bombs activation flag
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetPlasmaBombs(bool NewPlasmaBombs);

	// Returns the homing missiles activation flag
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	bool GetHomingMissiles() const;

	// Sets the value for the homing missiles activation flag
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetHomingMissiles(bool NewHomingMissiles);

	// Returns the seeker missiles activation flag
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	bool GetSeekerMissiles() const;

	// Sets the value for the seeker missiles activation flag
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetSeekerMissiles(bool NewSeekerMissiles);

private:

	// The player's current state
	EPlayerState CurrentState;

	// Power-up to be activated
	EPowerUp PowerUp;

	// Flag which indicates that the player is in power-up activation mode
	bool PowerUpActivationMode;

	// Score
	int32 GameScore;

	// Remaining lives
	uint8 Lives;

	// Speed up level (0 - 4)
	uint8 SpeedUpLevel;

	// Laser power (0 - 4)
	uint8 LaserPower;

	// Plasma bombs activation flag
	bool PlasmaBombs;

	// Homing missiles activation flag
	bool HomingMissiles;

	// Seeker missiles
	bool SeekerMissiles;

};
