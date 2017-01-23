// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "CustomGameConfig.generated.h"

/**
 * An object storing custom game configuration.
 */
UCLASS(Config = Game)
class ZYNAPSRELOADED_API UCustomGameConfig : public UObject
{
	GENERATED_BODY()

public:

	// true if the graphics settings were previosly initilized, false if the graphics have not been initialized,
	// i.e. the game is running for the first time
	UPROPERTY(Config)
	bool bGraphicsInitialized;
};
