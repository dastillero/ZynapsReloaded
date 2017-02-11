// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsCameraManager.h"
#include "ZynapsWorldSettings.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsCameraManager);

// Performs per-tick camera update
void AZynapsCameraManager::UpdateCamera(float DeltaSeconds)
{
	Super::UpdateCamera(DeltaSeconds);

	// Get game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsCameraManager, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// Don't move the camera if the game is in Preparing state
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

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
		UE_LOG(LogZynapsCameraManager, Error, TEXT("Failed to retrieve the Zynaps stage world settings"));
		return;
	}
	float CameraSpeed = WorldSettings->ScrollSpeed;
	GetViewTarget()->AddActorWorldOffset(FVector(0.0f, CameraSpeed * DeltaSeconds, 0.0f));
}

// Sets the camera location
void AZynapsCameraManager::SetCameraLocation(FVector Location)
{
	// Set the camera location
	GetViewTarget()->SetActorLocation(Location);
}

// Sets the camera location taking into account the world fixed camera offset
void AZynapsCameraManager::SetCameraLocationWithOffset(FVector Location)
{
	// Get the world settings
	AZynapsWorldSettings* ZynapsWorldSettings = AZynapsWorldSettings::GetZynapsWorldSettings(GetWorld());
	if (!ZynapsWorldSettings)
	{
		UE_LOG(LogZynapsCameraManager, Error, TEXT("Failed to retrieve the world settings"));
		return;
	}

	// Set the camera location plus the fixed camera offset
	GetViewTarget()->SetActorLocation(Location + ZynapsWorldSettings->FixedCameraOffset);
}

// Returns the game state
AZynapsGameState* AZynapsCameraManager::GetZynapsGameState() const
{
	return GetWorld()->GetGameState<AZynapsGameState>();
}
