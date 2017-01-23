// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "Projector2DComponent.h"


// Sets default values for this component's properties
UProjector2DComponent::UProjector2DComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UProjector2DComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UProjector2DComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Update viewport size and scene view
	UpdateViewportSize();
	UpdateSceneView();
}

// Called each tick to update the viewport size
void UProjector2DComponent::UpdateViewportSize()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		int32 Width;
		int32 Height;
		Controller->GetViewportSize(Width, Height);
		ViewportSize = FVector2D(Width, Height);
	}
}

// Called each tick to update the scene view
void UProjector2DComponent::UpdateSceneView()
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (Controller)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Controller->Player);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			FSceneViewFamily ViewFamily(FSceneViewFamily::ConstructionValues(
				LocalPlayer->ViewportClient->Viewport,
				GetWorld()->Scene,
				LocalPlayer->ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true));

			FVector ViewLocation;
			FRotator ViewRotation;
			SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation,
				LocalPlayer->ViewportClient->Viewport);
		}
	}
}

// Returns the viewport size
FVector2D UProjector2DComponent::GetViewportSize() const
{
	return ViewportSize;
}

// Converts the specified vector to its projection in screen coordinates
FVector2D UProjector2DComponent::ConvertToScreenCoordinates(FVector Vector) const
{
	FVector2D Result = FVector2D::ZeroVector;
	if (SceneView)
	{
		SceneView->WorldToPixel(Vector, Result);
	}
	return Result;
}

// Returns the actor's position in screen coordinates
FVector2D UProjector2DComponent::GetLocationInScreenCoordinates() const
{
	return ConvertToScreenCoordinates(GetOwner()->GetActorLocation());
}

// Returns the actor's size in screen coordinates, including only the colliding components.
FVector2D UProjector2DComponent::GetSizeInScreenCoordinates() const
{
	FVector Origin;
	FVector Extents;
	GetOwner()->GetActorBounds(true, Origin, Extents);
	FVector V0 = Origin - Extents;
	FVector V1 = Origin + Extents;
	FVector2D LeftVector = ConvertToScreenCoordinates(V0);
	FVector2D RightVector = ConvertToScreenCoordinates(V1);
	float SizeX = FMath::Abs(RightVector.X - LeftVector.X);
	float SizeY = FMath::Abs(RightVector.Y - LeftVector.Y);
	return FVector2D(SizeX, SizeY);
}
