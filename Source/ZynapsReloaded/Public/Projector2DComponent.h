// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Components/ActorComponent.h"
#include "Projector2DComponent.generated.h"

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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
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
	UFUNCTION(BlueprintPure, meta = (BlueprintProtected), Category = Util)
	FVector2D GetSizeInScreenCoordinates() const;

private:

	// Called each tick to update the viewport size
	void UpdateViewportSize();

	// Called each tick to update the scene view
	void UpdateSceneView();

	// Viewport size updated each tick
	FVector2D ViewportSize;

	// Scene view used to convert from world coordinates to screen coordinates
	FSceneView* SceneView;
};
