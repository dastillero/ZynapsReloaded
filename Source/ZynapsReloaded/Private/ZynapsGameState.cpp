// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsGameState.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsGameState);

// Default constructor
AZynapsGameState::AZynapsGameState() : Super()
{
	// Init the stage state
	SetCurrentState(EStageState::Preparing);
}

// Returns the current game state
EStageState AZynapsGameState::GetCurrentState() const
{
	return CurrentState;
}

// Sets the current stage state
void AZynapsGameState::SetCurrentState(EStageState State)
{
	// Change to the new state only if it is different from the current one
	if (CurrentState != State)
	{
		CurrentState = State;

		// Handle the new state
		switch (State)
		{
		case EStageState::Preparing:
			UE_LOG(LogZynapsGameState, Verbose, TEXT("Setting new game state: Preparing"));
			break;
		case EStageState::Playing:
			UE_LOG(LogZynapsGameState, Verbose, TEXT("Setting new game state: Playing"));
			break;
		case EStageState::GameOver:
			UE_LOG(LogZynapsGameState, Verbose, TEXT("Setting new game state: GameOver"));
			break;
		default:
			// Do nothing here
			UE_LOG(LogZynapsGameState, Warning, TEXT("Tried to set and invalid game state"));
			break;
		}
	}
}
