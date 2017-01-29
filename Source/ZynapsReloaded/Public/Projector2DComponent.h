// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Components/ActorComponent.h"
#include "Projector2DComponent.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogProjector2DComponent, Log, All);

/**
 * A reusable component to convert 3D information into 2D coordinates on the screen.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZYNAPSRELOADED_API UProjector2DComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjector2DComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaSeconds, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the viewport size
	UFUNCTION(BlueprintPure, Category = Util)
	FVector2D GetViewportSize() const;

	// Converts the specified vector to its projection in screen coordinates
	UFUNCTION(BlueprintPure, Category = Util)
	FVector2D ConvertToScreenCoordinates(FVector Vector) const;

	// Returns the position of the actor containing this component in screen coordinates
	UFUNCTION(BlueprintPure, Category = Util)
	FVector2D GetLocationInScreenCoordinates() const;

	// Returns the actor's size in screen coordinates, including only the colliding components.
	UFUNCTION(BlueprintPure, Category = Util)
	FVector2D GetSizeInScreenCoordinates() const;

	// Converts a screen coordinate to a 3D world location
	UFUNCTION(BlueprintPure, Category = Util)
	FVector ConvertFromScreenCoordinates(FVector2D Vector, float ViewingDistance) const;

	// Returns the player's camera distance and aspect ratio. Returns false they cannot be obtained.
	UFUNCTION(BlueprintPure, Category = Util)
	bool GetCameraDistanceAndAspectRatio(float& CameraDistance, float& CameraAspectRatio) const;
};
