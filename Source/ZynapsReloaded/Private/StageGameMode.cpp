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

// Called when the game starts
void AStageGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all player start objects in the stage
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		// Store the player start for later use
		PlayerStarts.Add(PlayerStart);

		// Identify the initial player start
		if (PlayerStart->GetName() == "StageInit")
		{
			StageInitPlayerStart = PlayerStart;
		}
	}

	// Check that the stage has player start objects defined
	if (PlayerStarts.Num() == 0)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("No player start actors found in the stage"));
		return;
	}
	UE_LOG(LogStageGameMode, Verbose, TEXT("%d player start actors found in the stage"), PlayerStarts.Num());

	// Check that the stage has an initial player start
	if (!StageInitPlayerStart)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("The stage must have a player start named StageInit"));
		return;
	}

	// Sort the player start object array using the Y coordinate
	PlayerStarts.Sort([](const APlayerStart& PlayerStart1, const APlayerStart& PlayerStart2)
	{
		FVector Location1 = PlayerStart1.GetActorLocation();
		FVector Location2 = PlayerStart2.GetActorLocation();
		return Location1.Y < Location2.Y;
	});

	// Set the current start for the player
	AZynapsController* ZynapsController = GetZynapsController();
	if (!ZynapsController)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player controller"));
		return;
	}
	ZynapsController->StartSpot = StageInitPlayerStart;

	// Set initial state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the stage state"));
		return;
	}
	ZynapsGameState->SetCurrentState(EStageState::Preparing);
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

	// Get the player controller
	AZynapsController* Controller = GetZynapsController();
	if (!Controller)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player controller"));
		return;
	}

	// Evaluate the current player start
	APlayerStart* NewPlayerStart = EvaluatePlayerStartSpot();
	if (!NewPlayerStart)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("The player start spot could not be evaluated"));
		return;
	}
	if (Controller->StartSpot == nullptr || Controller->StartSpot != NewPlayerStart)
	{
		Controller->StartSpot = NewPlayerStart;
	}

	// Handle stage states
	switch (ZynapsGameState->GetCurrentState())
	{
	case EStageState::Preparing:
		HandlePreparingState(ZynapsGameState, PlayerState, Controller);
		break;
	case EStageState::Playing:
		HandlePlayingState(ZynapsGameState, PlayerState, Controller);
		break;
	case EStageState::GameOver:
		HandleGameOverState(ZynapsGameState, PlayerState, Controller);
		break;
	}
}

// Called from Tick() to evaluate the player start to be used when the player is respawned
APlayerStart* AStageGameMode::EvaluatePlayerStartSpot()
{
	AZynapsCameraManager* CameraManager = GetZynapsCameraManager();
	if (!CameraManager)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the camera manager"));
		return nullptr;
	}

	APlayerStart* NewPlayerStart = StageInitPlayerStart;
	FVector CameraLocation = CameraManager->GetCameraLocation();
	for (APlayerStart* PlayerStart : PlayerStarts)
	{
		FVector PlayerStartLocation = PlayerStart->GetActorLocation();
		UE_LOG(LogStageGameMode, VeryVerbose,
			TEXT("Evaluating player start %s at %f against camera location at %f"),
			*PlayerStart->GetName(), PlayerStartLocation.Y, CameraLocation.Y);
		if (CameraLocation.Y > PlayerStartLocation.Y)
		{
			NewPlayerStart = PlayerStart;
		}
	}
	UE_LOG(LogStageGameMode, VeryVerbose, TEXT("Current player start evaluated to %s"),
		*NewPlayerStart->GetName());
	return NewPlayerStart;
}

// Handles the Preparing state
void AStageGameMode::HandlePreparingState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
	AZynapsController* ZynapsController)
{
	// Start playing after a given time
	if (!PreparingTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(PreparingTimerHandle))
	{
		GetWorldTimerManager().SetTimer(PreparingTimerHandle, this, &AStageGameMode::Play, PreparingDelay);
	}
}

// Handles the Playing state
void AStageGameMode::HandlePlayingState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
	AZynapsController* ZynapsController)
{
	// Update the state of the stage based on the state of the player
	if (ZynapsPlayerState->GetCurrentState() == EPlayerState::Destroyed)
	{
		// The player has been destroyed
		if (!PlayerCanRestart(ZynapsController))
		{
			// No more lives available
			ZynapsGameState->SetCurrentState(EStageState::GameOver);
		}
		else
		{
			// More lives available, set a timed respawn if it was not set previously
			if (!SpawnTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
			{
				GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AStageGameMode::Respawn, RespawnDelay);
			}
		}
	}
	else
	{
		// The player is playing
		ZynapsGameState->SetCurrentState(EStageState::Playing);
	}
}

// Handles the GameOver state
void AStageGameMode::HandleGameOverState(AZynapsGameState* ZynapsGameState, AZynapsPlayerState* ZynapsPlayerState,
	AZynapsController* ZynapsController)
{
	// Go back to the main menu after a given time
	if (!GameOverTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(GameOverTimerHandle))
	{
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &AStageGameMode::ExitToMenu, GameOverDelay);
	}
}

// Sets the state state to Playing
void AStageGameMode::Play()
{
	// Get the stage state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// Set the stage state to Playing
	ZynapsGameState->SetCurrentState(EStageState::Playing);
}

// Respawn the player pawn
void AStageGameMode::Respawn()
{
	// Get the stage state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the game state. The player won't be respawned"));
		return;
	}

	// Get the player controller
	AZynapsController* ZynapsController = GetZynapsController();
	if (!ZynapsController)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the player controller. The player won't be respawned"));
		return;
	}

	// Get the camera manager
	AZynapsCameraManager* ZynapsCameraManager = GetZynapsCameraManager();
	if (!ZynapsCameraManager)
	{
		UE_LOG(LogStageGameMode, Error, TEXT("Failed to retrieve the camera manager. The player won't be respawned"));
		return;
	}

	// Set the camera location back to the current player start
	FVector CameraLocation = ZynapsCameraManager->GetCameraLocation();
	APlayerStart* PlayerStart = Cast<APlayerStart>(ZynapsController->StartSpot.Get());
	if (!PlayerStart)
	{
		PlayerStart = StageInitPlayerStart;
	}
	FVector PlayerStartLocation = PlayerStart->GetActorLocation();
	CameraLocation.Y = PlayerStartLocation.Y;
	ZynapsCameraManager->SetCameraLocation(CameraLocation);

	// Respawn the player and set the stage state to Preparing
	AZynapsController* Controller = GetZynapsController();
	UE_LOG(LogStageGameMode, Verbose, TEXT("Respawning player at %s"), *Controller->StartSpot->GetName());
	RestartPlayer(Controller);
	ZynapsGameState->SetCurrentState(EStageState::Preparing);
}

// Go back to the main menu
void AStageGameMode::ExitToMenu()
{
	// Go back to the menu
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("World'/Game/Levels/Menu.Menu'"));
}

// Returns the player's pawn
APlayerPawn* AStageGameMode::GetPlayerPawn() const 
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Pawn)
	{
		return nullptr;
	}
	return Cast<APlayerPawn>(Pawn);
}

// Retrieves the game state
AZynapsGameState* AStageGameMode::GetZynapsGameState() const
{
	if (!GameState)
	{
		return nullptr;
	}
	return Cast<AZynapsGameState>(GameState);
}

// Returns the player controller
AZynapsController* AStageGameMode::GetZynapsController() const
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (!Controller)
	{
		return nullptr;
	}
	return Cast<AZynapsController>(Controller);
}

// Returns the player state
AZynapsPlayerState* AStageGameMode::GetZynapsPlayerState() const
{
	AZynapsController* Controller = GetZynapsController();
	if (!Controller)
	{
		return nullptr;
	}

	APlayerState* PlayerState = Controller->PlayerState;
	if (!PlayerState)
	{
		return nullptr;
	}

	return Cast<AZynapsPlayerState>(PlayerState);
}

// Returns the camera manager
AZynapsCameraManager* AStageGameMode::GetZynapsCameraManager() const
{
	AZynapsController* Controller = GetZynapsController();
	if (!Controller)
	{
		return nullptr;
	}

	APlayerCameraManager* CameraManager = Controller->PlayerCameraManager;
	if (!CameraManager)
	{
		return nullptr;
	}

	return Cast<AZynapsCameraManager>(CameraManager);
}