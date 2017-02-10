// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsCameraManager.h"
#include "ZynapsWorldSettings.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsCameraManager);

// Sets default values for the controller
AZynapsCameraManager::AZynapsCameraManager() : Super()
{
}

// Performs per-tick camera update
void AZynapsCameraManager::UpdateCamera(float DeltaSeconds)
{
	Super::UpdateCamera(DeltaSeconds);

	// Get the camera
	AActor* Camera = GetViewTarget();
	if (!Camera)
	{
		UE_LOG(LogZynapsCameraManager, Error, TEXT("Failed to retrieve the view target"));
		return;
	}

	// Scroll the camera using the speed in the world settings
	AZynapsWorldSettings* WorldSettings = AZynapsWorldSettings::GetZynapsWorldSettings(GetWorld());
	if (!WorldSettings)
	{
		UE_LOG(LogZynapsCameraManager, Warning, TEXT("Failed to retrieve the Zynaps stage world settings"));
		return;
	}
	float CameraSpeed = WorldSettings->ScrollSpeed;
	GetViewTarget()->AddActorWorldOffset(FVector(0.0f, CameraSpeed * DeltaSeconds, 0.0f));
}

// Sets the camera location
void AZynapsCameraManager::SetCameraLocation(FVector Location)
{
	GetViewTarget()->SetActorLocation(Location);
}
