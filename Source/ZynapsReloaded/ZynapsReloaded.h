// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Engine.h"

// Macro for showing messages on the screen in an easy way
#define PRINT(FormatString, ...) \
	if (GEngine) \
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, \
			FString::Printf(TEXT(FormatString), ##__VA_ARGS__))

// Global log categories
DECLARE_LOG_CATEGORY_EXTERN(LogZynaps, Log, All);
