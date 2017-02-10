// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/GameModeBase.h"
#include "PlayerPawn.h"
#include "ZynapsCameraManager.h"
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

	// Called when the game starts
	virtual void BeginPlay() override;
	
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

	// Returns the camera manager
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	AZynapsCameraManager* GetZynapsCameraManager() const;


private:

	// Timer handle which manages the time before spawning the player after it was destroyed
	FTimerHandle SpawnTimerHandle;

	// Player start objects in the stage
	UPROPERTY()  // Needed to ensure garbage collection
	TArray<APlayerStart*> PlayerStarts;

	// Player start which marks the stage init
	APlayerStart* StageInitPlayerStart;

};
