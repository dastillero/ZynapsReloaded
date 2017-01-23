// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "StageGameMode.h"
#include "ZynapsController.h"
#include "ZynapsPlayerState.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStageGameMode::AStageGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Sets the default pawn
	DefaultPawnClass = APlayerPawn::StaticClass();

	// Sets the default player controller class
	PlayerControllerClass = AZynapsController::StaticClass();

	// Sets the default player state class
	PlayerStateClass = AZynapsPlayerState::StaticClass();

	// Sets the initial stage state
	SetCurrentState(EStageState::EPreparing);
}

// Called every frame
void AStageGameMode::Tick(float DeltaSeconds)
{
	// Get the player's state
	AZynapsPlayerState* PlayerState = GetPlayerPawn()->GetZynapsPlayerState();
	if (PlayerState == NULL)
	{
		return;
	}

	// Update the stage state
	if (PlayerState->Lives == 0)
	{
		SetCurrentState(EStageState::EGameOver);
	}
}

// Returns the player's pawn
APlayerPawn* AStageGameMode::GetPlayerPawn() const 
{
	return Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Returns the current game state
EStageState AStageGameMode::GetCurrentState() const
{
	return CurrentState;
}

// Sets the current stage state
void AStageGameMode::SetCurrentState(EStageState State)
{
	// Change to the new state only if it is different from the current one
	if (CurrentState != State)
	{
		CurrentState = State;

		// Handle the new state
		HandleNewState(State);
	}
}

// Handles a new stage when it is set
void AStageGameMode::HandleNewState(EStageState State) 
{
	switch (State)
	{
	case EStageState::EPreparing:
		break;
	case EStageState::EPlaying:
		break;
	case EStageState::EGameOver:
		break;
	default:
		// Do nothing here
		break;
	}
}

