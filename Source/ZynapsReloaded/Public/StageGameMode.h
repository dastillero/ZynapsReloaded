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

// Preparing delay
const float PreparingDelay = 2.5f;

// Game over delay
const float GameOverDelay = 4.0f;

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

	// Implementation which returns the StageInit player start
	AActor* ChoosePlayerStart_Implementation(AController* Controller) override;

protected:

	// Called from Tick() to evaluate the player start to be used when the player is respawned
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	APlayerStart* EvaluatePlayerStartSpot();

	// Handles the Preparing state
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void HandlePreparingState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
		AZynapsController* ZynapsController);

	// Handles the Playing state
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void HandlePlayingState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
		AZynapsController* ZynapsController);

	// Handles the GameOver state
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsState)
	void HandleGameOverState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
		AZynapsController* ZynapsController);

	// Sets the state state to Playing
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void Play();

	// Respawn the player's pawn
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void Respawn();

	// Go back to the main menu
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void ExitToMenu();

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

	// Timer handle which manages the time before the game starts regular playing
	FTimerHandle PreparingTimerHandle;

	// Timer handle which manages the time before spawning the player after it has been destroyed
	FTimerHandle SpawnTimerHandle;

	// Timer handle which manages the time before the game goes back to the main menu when the game is over
	FTimerHandle GameOverTimerHandle;

	// Player start objects in the stage
	UPROPERTY()  // Needed to ensure garbage collection
	TArray<APlayerStart*> PlayerStarts;

	// Player start which marks the stage init
	APlayerStart* StageInitPlayerStart;

};
