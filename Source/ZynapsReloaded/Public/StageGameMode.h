// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/GameModeBase.h"
#include "PlayerPawn.h"
#include "StageGameMode.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogStageGameMode, Log, All);

// Respawn delay
const float RespawnDelay = 2.0f;

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

	// Respawn the player pawn
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void Respawn();

	// Retrieves the player's pawn
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	APlayerPawn* GetPlayerPawn() const;

	// Retrieves the game state
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsGameState* GetZynapsGameState() const;

	// Returns the player controller
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsController* GetZynapsController() const;

	// Returns the player state
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsPlayerState* GetZynapsPlayerState() const;

private:

	// Timer handle which manages the time before spawning the player after it was destroyed
	FTimerHandle SpawnTimerHandle;

};
