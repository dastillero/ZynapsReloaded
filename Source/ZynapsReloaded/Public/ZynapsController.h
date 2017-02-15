// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerPawn.h"
#include "ZynapsController.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsController, Log, All);

// Time to activate the power-up activation mode
const double PowerUpActivationModeTime = 0.25;  // A quarter of second

/**
 * The default Player Controller used by StageGameMode.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsController : public APlayerController
{
	GENERATED_BODY()
	
public:

	// Sets default values for the controller
	AZynapsController(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Returns true if the player has more lives available. false otherwise.
	virtual bool CanRestartPlayer() override;
	
protected:

	// Sets up input bindings
	virtual void SetupInputComponent() override;
	
	// Handles moving up
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void MoveUp(float Val);

	// Handles moving down
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void MoveDown(float Val);

	// Handles moving left
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void MoveLeft(float Val);

	// Handles moving right
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void MoveRight(float Val);

	// Handles fire button pressed
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void FirePressed();

	// Handles fire button released
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void FireReleased();

	// Handles back button pressed
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = ZynapsActions)
	void BackPressed();

private:

	// Returns the player's pawn
	APlayerPawn* GetPlayerPawn() const;

	// Returns the game state
	AZynapsGameState* GetZynapsGameState() const;

	// Returns the player state
	AZynapsPlayerState* GetZynapsPlayerState() const;

	// Time in which the fire button was pressed
	double FirePressedTime;
};
