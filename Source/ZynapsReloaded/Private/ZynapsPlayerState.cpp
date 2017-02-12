// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsPlayerState.h"
#include "UnrealNetwork.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsPlayerState);

// Default constructor
AZynapsPlayerState::AZynapsPlayerState() : Super()
{
	SetCurrentState(EPlayerState::Playing);
	GameScore = 0;
	Lives = 3;
	SpeedUpLevel = 0;
	LaserPower = 0;
	PlasmaBombs = false;
	HomingMissiles = false;
	SeekerMissiles = false;
	PowerUpIndex = 0;
	PowerUpActivationMode = false;
}

// Returns the set of replicated properties
void AZynapsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZynapsPlayerState, CurrentState);
	DOREPLIFETIME(AZynapsPlayerState, GameScore);
	DOREPLIFETIME(AZynapsPlayerState, Lives);
	DOREPLIFETIME(AZynapsPlayerState, SpeedUpLevel);
	DOREPLIFETIME(AZynapsPlayerState, LaserPower);
	DOREPLIFETIME(AZynapsPlayerState, GameScore);
	DOREPLIFETIME(AZynapsPlayerState, PlasmaBombs);
	DOREPLIFETIME(AZynapsPlayerState, HomingMissiles);
	DOREPLIFETIME(AZynapsPlayerState, SeekerMissiles);
}

// Returns the current player state
EPlayerState AZynapsPlayerState::GetCurrentState() const
{
	return CurrentState;
}

// Sets the current player state
void AZynapsPlayerState::SetCurrentState(EPlayerState State)
{
	// Change to the new state only if it is different from the current one
	if (CurrentState != State)
	{
		// Handle the new state
		switch (State)
		{
		case EPlayerState::Playing:
			UE_LOG(LogZynapsPlayerState, Verbose, TEXT("Setting new player state: Playing"));
			CurrentState = State;
			break;
		case EPlayerState::Destroyed:
			UE_LOG(LogZynapsPlayerState, Verbose, TEXT("Setting new player state: Destroyed"));
			CurrentState = State;
			ReduceLives();
			break;
		default:
			// Do nothing here
			UE_LOG(LogZynapsPlayerState, Warning, 
				TEXT("Tried to set and invalid player state. The state will not be changed"));
			break;
		}
	}
}

// Reduces a live and resets the state
void AZynapsPlayerState::ReduceLives()
{
	Lives--;
	SpeedUpLevel = 0;
	LaserPower = 0;
	PlasmaBombs = false;
	HomingMissiles = false;
	SeekerMissiles = false;
}

// Returns the index of the power-up to be activated
uint8 AZynapsPlayerState::GetPowerUpIndex() const
{
	return PowerUpIndex;
}

// Called when a fuel capsule is collected
void AZynapsPlayerState::FuelCapsuleCollected()
{
	if (!PowerUpActivationMode)
	{
		ShiftPowerUpIndex();
	}
}

// Cycles through the power-up indexes
void AZynapsPlayerState::ShiftPowerUpIndex()
{
	if (++PowerUpIndex > 4)
	{
		PowerUpIndex = 0;
	}
}
