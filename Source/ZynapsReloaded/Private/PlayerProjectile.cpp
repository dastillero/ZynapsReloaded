// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "PlayerProjectile.h"
#include "ProjectionUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogPlayerProjectile);

// Sets default values
APlayerProjectile::APlayerProjectile() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Destroy the projectile if it is not visible anymore
	if (!IsVisibleOnScreen())
	{
		Destroy();
	}
}

// Checks that the projectile is within the viewport limits
bool APlayerProjectile::IsVisibleOnScreen() const
{
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogPlayerProjectile, Error, TEXT("Failed to retrieve the player controller"));
		return false;
	}

	// Check that the projectile is within screen limits
	AActor* Actor = (AActor*)this;
	FVector2D Position = UProjectionUtil::GetLocationInScreenCoordinates(PlayerController, Actor);
	FVector2D Size = UProjectionUtil::GetSizeInScreenCoordinates(PlayerController, Actor);
	FVector2D ViewportSize = UProjectionUtil::GetViewportSize(PlayerController);
	if (Position.X + Size.X / 2 < 0 || Position.X - Size.X / 2 > ViewportSize.X ||
		Position.Y + Size.Y / 2 < 0 || Position.Y + Size.Y / 2 > ViewportSize.Y)
	{
		return false;
	}
	return true;
}