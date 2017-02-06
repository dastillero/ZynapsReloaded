// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "StageGameMode.h"
#include "ZynapsGameState.h"
#include "ZynapsController.h"
#include "ZynapsPlayerState.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

// Log category
DEFINE_LOG_CATEGORY(LogStageGameMode);

// Sets default values
AStageGameMode::AStageGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets the game state class
	GameStateClass = AZynapsGameState::StaticClass();

	// Sets the default pawn
	DefaultPawnClass = APlayerPawn::StaticClass();

	// Sets the default player controller class
	PlayerControllerClass = AZynapsController::StaticClass();

	// Sets the default player state class
	PlayerStateClass = AZynapsPlayerState::StaticClass();
}

// Called every frame
void AStageGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Get the stage state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// Get the player's state
	AZynapsPlayerState* PlayerState = GetZynapsPlayerState();
	if (!PlayerState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player state"));
		return;
	}

	// Update the state of the stage based on the state of the player
	if (PlayerState->GetCurrentState() == EPlayerState::Destroyed)
	{
		// The player has been destroyed
		if (PlayerState->Lives == 0)
		{
			// No more lives available
			ZynapsGameState->SetCurrentState(EStageState::GameOver);
		}
		else
		{
			// More lives available
			ZynapsGameState->SetCurrentState(EStageState::Preparing);
			RestartPlayer(GetWorld()->GetFirstPlayerController());
		}
	}
	else
	{
		// The player is playing
		ZynapsGameState->SetCurrentState(EStageState::Playing);
	}
}

// Returns the player's pawn
APlayerPawn* AStageGameMode::GetPlayerPawn() const 
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Pawn)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player's pawn"));
		return nullptr;
	}
	return Cast<APlayerPawn>(Pawn);
}

// Retrieves the game state
AZynapsGameState* AStageGameMode::GetZynapsGameState() const
{
	if (!GameState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the game state"));
		return nullptr;
	}
	return Cast<AZynapsGameState>(GameState);
}

// Returns the player state
AZynapsPlayerState* AStageGameMode::GetZynapsPlayerState() const
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (!Controller)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player controller"));
		return nullptr;
	}

	APlayerState* PlayerState = Controller->PlayerState;
	if (!PlayerState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player state"));
		return nullptr;
	}

	return Cast<AZynapsPlayerState>(PlayerState);
}
