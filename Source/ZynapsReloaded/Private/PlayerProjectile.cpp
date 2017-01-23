// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "PlayerProjectile.h"


// Sets default values
APlayerProjectile::APlayerProjectile() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Set up the Projector 2D component
	Projector2DComponent = CreateDefaultSubobject<UProjector2DComponent>(TEXT("Projector2DComponent"));
}

// Called when the game starts or when spawned
void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Destroy the projectile if it is not visible anymore
	if (!IsVisibleOnScreen())
	{
		Destroy();
	}
}

// Checks that the projectile is within the viewport limits
bool APlayerProjectile::IsVisibleOnScreen() const
{
	FVector2D Position = Projector2DComponent->GetLocationInScreenCoordinates();
	FVector2D Size = Projector2DComponent->GetSizeInScreenCoordinates();
	FVector2D ViewportSize = Projector2DComponent->GetViewportSize();
	if (Position.X + Size.X / 2 < 0 || Position.X - Size.X / 2 > ViewportSize.X ||
		Position.Y + Size.Y / 2 < 0 || Position.Y + Size.Y / 2 > ViewportSize.Y)
	{
		return false;
	}
	return true;
}