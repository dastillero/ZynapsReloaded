// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Camera/PlayerCameraManager.h"
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

	// Sets default values for the controller
	AZynapsCameraManager();

	// Performs per-tick camera update
	virtual void UpdateCamera(float DeltaSeconds);

	// Returns the camera speed
	UFUNCTION(BlueprintPure, Category = Camera)
	float GetCameraSpeed() const;

	// Sets the camera speed
	UFUNCTION(BlueprintCallable, Category = Camera)
	void SetCameraSpeed(float NewCameraSpeed);

private:

	// Camera scroll speed
	float CameraSpeed;
};
