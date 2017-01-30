// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayerPawn.h"
#include "ZynapsController.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsController, Log, All);

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

	// Returns the player's pawn
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = ZynapsState)
	APlayerPawn* GetPlayerPawn() const;

};
