// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "GameFramework/WorldSettings.h"
#include "ZynapsWorldSettings.generated.h"

/**
 * Specific settings for Zynaps stages.
 */
UCLASS()
class ZYNAPSRELOADED_API AZynapsWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:

	// Sets default values
	AZynapsWorldSettings();

	// Returns the world settings for the game for the specified world
	static AZynapsWorldSettings* GetZynapsWorldSettings(UWorld* World);

	// Scroll speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
	float ScrollSpeed;
	
};
