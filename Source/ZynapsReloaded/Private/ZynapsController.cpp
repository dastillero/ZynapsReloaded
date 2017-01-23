// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ZynapsController.h"

// Sets default values for the controller
AZynapsController::AZynapsController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

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
}

// Handles moving up
void AZynapsController::MoveUp(float Val)
{
	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveUp(Val);
	}
}

// Handles moving down
void AZynapsController::MoveDown(float Val)
{
	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveDown(Val);
	}
}

// Handles moving left
void AZynapsController::MoveLeft(float Val)
{
	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveLeft(Val);
	}
}

// Handles moving right
void AZynapsController::MoveRight(float Val)
{
	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL && Val != 0.0f)
	{
		PlayerPawn->MoveRight(Val);
	}
}

// Handles fire button pressed
void AZynapsController::FirePressed()
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AZynapsController::FirePressed"));
	}

	APlayerPawn* PlayerPawn = GetPlayerPawn();
	if (PlayerPawn != NULL)
	{
		PlayerPawn->Fire();
	}
}

// Handles fire button released
void AZynapsController::FireReleased()
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("AZynapsController::FireReleased"));
	}
}

// Returns the player's pawn
APlayerPawn* AZynapsController::GetPlayerPawn() const
{
	return Cast<APlayerPawn>(GetPawn());
}
