// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsController.h"
#include "ZynapsCameraManager.h"

// Log category
DEFINE_LOG_CATEGORY(LogZynapsController);

// Sets default values for the controller
AZynapsController::AZynapsController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set the player camera manager
	PlayerCameraManagerClass = AZynapsCameraManager::StaticClass();
}

// Called when the game starts
void AZynapsController::BeginPlay()
{
	Super::BeginPlay();
}

// Returns true if the player has more lives available. false otherwise.
bool AZynapsController::CanRestartPlayer()
{
	if (!PlayerState)
	{
		return false;
	}

	AZynapsPlayerState* ZynapsPlayerState = Cast<AZynapsPlayerState>(PlayerState);
	if (ZynapsPlayerState->Lives > 0)
	{
		return true;
	}

	return false;
}

// Called to bind functionality to input
void AZynapsController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind controls
	InputComponent->BindAxis("MoveUp", this, &AZynapsController::MoveUp);
	InputComponent->BindAxis("MoveDown", this, &AZynapsController::MoveDown);
	InputComponent->BindAxis("MoveLeft", this, &AZynapsController::MoveLeft);
	InputComponent->BindAxis("MoveRight", this, &AZynapsController::MoveRight);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AZynapsController::FirePressed);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AZynapsController::FireReleased);
	InputComponent->BindAction("Back", EInputEvent::IE_Pressed, this, &AZynapsController::BackPressed);
}

// Handles moving up
void AZynapsController::MoveUp(float Val)
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't move the pawn
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveUp(Val);
	}
}

// Handles moving down
void AZynapsController::MoveDown(float Val)
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't move the pawn
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveDown(Val);
	}
}

// Handles moving left
void AZynapsController::MoveLeft(float Val)
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't move the pawn
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveLeft(Val);
	}
}

// Handles moving right
void AZynapsController::MoveRight(float Val)
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't move the pawn
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveRight(Val);
	}
}

// Handles fire button pressed
void AZynapsController::FirePressed()
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't fire
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn)
	{
		PlayerPawn->Fire();
	}
}

// Handles fire button released
void AZynapsController::FireReleased()
{
	// Get the game state
	AZynapsGameState* ZynapsGameState = GetZynapsGameState();
	if (!ZynapsGameState)
	{
		UE_LOG(LogZynapsController, Error, TEXT("Failed to retrieve the game state"));
		return;
	}

	// If the game is in Preparing state don't process anything
	if (ZynapsGameState->GetCurrentState() == EStageState::Preparing)
	{
		return;
	}
}

// Handles back button pressed
void AZynapsController::BackPressed()
{
	// Go back to the menu
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("World'/Game/Levels/Menu.Menu'"));
}

// Returns the player's pawn
APlayerPawn* AZynapsController::GetPlayerPawn() const
{
	return Cast<APlayerPawn>(GetPawn());
}

// Returns the game state
AZynapsGameState* AZynapsController::GetZynapsGameState() const
{
	return GetWorld()->GetGameState<AZynapsGameState>();
}
