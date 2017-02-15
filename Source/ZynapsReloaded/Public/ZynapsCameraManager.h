// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "ZynapsGameState.h"
#include "ZynapsCameraManager.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogZynapsCameraManager, Log, All);

/**
 * A camera manager to handle the scrolling camera in the game.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:

	// Performs per-tick camera update
	virtual void UpdateCamera(float DeltaSeconds);

	// Sets the camera location
	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraLocation(FVector Location);

	// Sets the camera location taking into account the world fixed camera offset
	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraLocationWithOffset(FVector Location);

private:

	// Returns the game state
	AZynapsGameState* GetZynapsGameState() const;
};
