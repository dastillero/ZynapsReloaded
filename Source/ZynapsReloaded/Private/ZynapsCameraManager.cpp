// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsCameraManager.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsCameraManager);

// Sets default values for the controller
AZynapsCameraManager::AZynapsCameraManager() : Super()
{
	// Set the default camera speed
	CameraSpeed = 1000.0f;
}

// Performs per-tick camera update
void AZynapsCameraManager::UpdateCamera(float DeltaSeconds)
{
	Super::UpdateCamera(DeltaSeconds);

	// Scroll the camera
	AActor* Camera = GetViewTarget();
	if (!Camera)
	{
		UE_LOG(LogZynapsCameraManager, Error, TEXT("Failed to retrieve view target"));
		return;
	}
	GetViewTarget()->AddActorWorldOffset(FVector(0.0f, CameraSpeed * DeltaSeconds, 0.0f));
}

// Returns the camera speed
float AZynapsCameraManager::GetCameraSpeed() const
{
	return CameraSpeed;
}

// Sets the camera speed
void AZynapsCameraManager::SetCameraSpeed(float NewCameraSpeed)
{
	CameraSpeed = NewCameraSpeed;
}

