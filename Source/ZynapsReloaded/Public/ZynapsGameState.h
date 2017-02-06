// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/GameStateBase.h"
#include "ZynapsGameState.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsGameState, Log, All);

/**
 * Enumeration storing the states of the stage during gameplay.
 */
UENUM(BlueprintType)
enum class EStageState : uint8
{
	Preparing = 0,
	Playing = 1,
	GameOver = 2
};

/**
 * Class which manages the state during a stage game mode.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	// Default constructor
	AZynapsGameState();

	// Returns the current game state
	UFUNCTION(BlueprintPure, Category = ZynapsState)
	EStageState GetCurrentState() const;

	// Sets the current game state
	UFUNCTION(BlueprintCallable, Category = ZynapsState)
	void SetCurrentState(EStageState State);

protected:

	// Handles a new state when it is set
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	virtual void HandleNewState(EStageState State);

private:

	// Current game state
	EStageState CurrentState;
	
};
