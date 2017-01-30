// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerState.h"
#include "ZynapsPlayerState.generated.h"

/**
 * This class stores the player's state information.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

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

};
