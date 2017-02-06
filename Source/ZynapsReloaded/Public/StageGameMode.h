// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/GameModeBase.h"
#include "PlayerPawn.h"
#include "StageGameMode.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogStageGameMode, Log, All);

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

protected:

	// Retrieves the player's pawn
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	APlayerPawn* GetPlayerPawn() const;

	// Retrieves the game state
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsGameState* GetZynapsGameState() const;

	// Returns the player state
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsPlayerState* GetZynapsPlayerState() const;
};
