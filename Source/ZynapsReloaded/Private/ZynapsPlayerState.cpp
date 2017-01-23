// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsPlayerState.h"
#include "UnrealNetwork.h"

// Returns the set of replicated properties
void AZynapsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZynapsPlayerState, GameScore);
	DOREPLIFETIME(AZynapsPlayerState, Lives);
	DOREPLIFETIME(AZynapsPlayerState, SpeedUpLevel);
	DOREPLIFETIME(AZynapsPlayerState, LaserPower);
	DOREPLIFETIME(AZynapsPlayerState, GameScore);
	DOREPLIFETIME(AZynapsPlayerState, PlasmaBombs);
	DOREPLIFETIME(AZynapsPlayerState, HomingMissiles);
	DOREPLIFETIME(AZynapsPlayerState, SeekerMissiles);
}


