// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsWorldSettings.h"

// Sets default values
AZynapsWorldSettings::AZynapsWorldSettings() : Super()
{
	// Default scroll speed
	ScrollSpeed = 1000.0f;

	// Default fixed camera offset
	FixedCameraOffset = FVector(0.0f, 2500.0f, 0.0f);
}

// Returns the world settings for the specified world
AZynapsWorldSettings* AZynapsWorldSettings::GetZynapsWorldSettings(UWorld* World)
{
	TActorIterator<AZynapsWorldSettings> It(World);
	if (It) return *It;
	return nullptr;
}

