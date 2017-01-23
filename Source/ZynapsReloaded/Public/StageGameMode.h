// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/GameModeBase.h"
#include "PlayerPawn.h"
#include "StageGameMode.generated.h"

/**
 * Enumeration storing the states of the stage during gameplay.
 */
UENUM()
enum class EStageState : uint8
{
	EPreparing = 0,
	EPlaying = 1,
	EGameOver = 2
};

/**
 * GameMode for a regular stage in the game.
 */
UCLASS()
class ZYNAPSRELOADED_API AStageGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for the GameMode
	AStageGameMode(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Returns the current game state
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	EStageState GetCurrentState() const;

	// Sets the current game state
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetCurrentState(EStageState State);

protected:
	// Retrieves the player's pawn
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	APlayerPawn* GetPlayerPawn() const;

	// Handles a new state when it is set
	virtual void HandleNewState(EStageState State);

private:
	// Current game state
	EStageState CurrentState;

};
