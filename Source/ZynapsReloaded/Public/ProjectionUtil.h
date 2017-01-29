// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectionUtil.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogProjectionUtil, Log, All);

/**
 * A library of static functions to perform convertions from 3D world coordinates to 2D screen coordinates and
 * viceversa.
 */
UCLASS()
class ZYNAPSRELOADED_API UProjectionUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Returns the viewport size for the given player controller
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FVector2D GetViewportSize(APlayerController* PlayerController);

	// Converts the specified vector to its projection in screen coordinates
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FVector2D ConvertToScreenCoordinates(APlayerController* PlayerController, FVector Vector);

	// Returns the position of the specified actor in screen coordinates
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FVector2D GetLocationInScreenCoordinates(APlayerController* PlayerController, AActor* Actor);

	// Returns the size of the specified actor size in screen coordinates, including only the colliding components.
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FVector2D GetSizeInScreenCoordinates(APlayerController* PlayerController, AActor* Actor);

	// Converts a screen coordinate to a 3D world location
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FVector ConvertFromScreenCoordinates(APlayerController* PlayerController, FVector2D Vector, 
		float ViewingDistance);

	// Returns the player's camera distance and aspect ratio. Returns false if they cannot be obtained.
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool GetCameraDistanceAndAspectRatio(APlayerController* PlayerController, float& CameraDistance, 
		float& CameraAspectRatio);

	// Calculates the viewport bounds excluding the black bars in letterboxed screen modes if neccesary. Returns
	// false on error.
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool CalculateViewportBounds(APlayerController* PlayerController, FVector& TopLeftBound,
			FVector& BottomRightBound);
};
