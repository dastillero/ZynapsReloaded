// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "ProjectionUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogProjectionUtil);

// Returns the viewport size for a given player controller
FVector2D UProjectionUtil::GetViewportSize(APlayerController* PlayerController)
{
	int32 Width;
	int32 Height;
	PlayerController->GetViewportSize(Width, Height);
	FVector2D Result = FVector2D(Width, Height);
	return Result;
}

// Converts the specified vector to its projection in screen coordinates
FVector2D UProjectionUtil::ConvertToScreenCoordinates(APlayerController* PlayerController, FVector Vector)
{
	FVector2D Result;
	PlayerController->ProjectWorldLocationToScreen(Vector, Result, false);
	return Result;
}

// Returns the position of the specified actor in screen coordinates
FVector2D UProjectionUtil::GetLocationInScreenCoordinates(APlayerController* PlayerController, AActor* Actor)
{
	return ConvertToScreenCoordinates(PlayerController, Actor->GetActorLocation());
}

// Returns the size of the specified actor size in screen coordinates, including only the colliding components.
FVector2D UProjectionUtil::GetSizeInScreenCoordinates(APlayerController* PlayerController, AActor* Actor)
{
	FVector Origin;
	FVector Extents;
	Actor->GetActorBounds(true, Origin, Extents);
	FVector V0 = Origin - Extents;
	FVector V1 = Origin + Extents;
	FVector2D LeftVector = ConvertToScreenCoordinates(PlayerController, V0);
	FVector2D RightVector = ConvertToScreenCoordinates(PlayerController, V1);
	float SizeX = FMath::Abs(RightVector.X - LeftVector.X);
	float SizeY = FMath::Abs(RightVector.Y - LeftVector.Y);
	return FVector2D(SizeX, SizeY);
}

// Converts a screen coordinate to a 3D world location
FVector UProjectionUtil::ConvertFromScreenCoordinates(APlayerController* PlayerController, FVector2D Vector,
	float ViewingDistance)
{
	FVector Location;
	FVector Direction;
	PlayerController->DeprojectScreenPositionToWorld(Vector.X, Vector.Y, Location, Direction);
	FVector Result = Location + (Direction * ViewingDistance);
	return Result;
}

// Returns the player's camera distance and aspect ratio. Returns false if they cannot be obtained.
bool UProjectionUtil::GetCameraDistanceAndAspectRatio(APlayerController* PlayerController, float& CameraDistance,
	float& CameraAspectRatio)
{
	if (PlayerController->PlayerCameraManager)
	{
		FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		CameraDistance = FMath::Abs(CameraLocation.X);
		AActor* ViewTarget = PlayerController->PlayerCameraManager->GetViewTarget();
		if (ViewTarget)
		{
			ACameraActor* Camera = Cast<ACameraActor>(ViewTarget);
			CameraAspectRatio = Camera->GetCameraComponent()->AspectRatio;
		}
		else
		{
			UE_LOG(LogProjectionUtil, Error, TEXT("The camera manager view target could not be retrieved"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogProjectionUtil, Error, TEXT("The camera manager could not be retrieved"));
		return false;
	}

	return true;
}

// Calculates the viewport bounds excluding the black bars in letterboxed screen modes if neccesary. Returns
// false on error.
bool UProjectionUtil::CalculateViewportBounds(APlayerController* PlayerController, FVector& TopLeftBound,
	FVector& BottomRightBound)
{
	// Get the camera distance and aspect ratio 
	float CameraDistance = 20000.0f;
	float CameraAspectRatio = 16.0f / 9.0f;
	if (!UProjectionUtil::GetCameraDistanceAndAspectRatio(PlayerController, CameraDistance, CameraAspectRatio))
	{
		UE_LOG(LogProjectionUtil, Warning, TEXT("The camera distance and aspect ratio could not be retrieved"));
		return false;
	}

	// Calculate the size of black bars when a non 16:9 resolution is set
	FVector2D ViewportSize = GetViewportSize(PlayerController);
	float ViewportAspectRatio = ViewportSize.X / ViewportSize.Y;
	float ScreenMinX = 0.0f;
	float ScreenMaxX = ViewportSize.X;
	float ScreenMinY = 0.0f;
	float ScreenMaxY = ViewportSize.Y;
	if (ViewportAspectRatio < CameraAspectRatio)
	{
		// Top and bottom black bars
		float HorizontalBlackBarHeight = (ViewportSize.Y - (ViewportSize.X / CameraAspectRatio)) / 2.0f;
		ScreenMinY += HorizontalBlackBarHeight;
		ScreenMaxY -= HorizontalBlackBarHeight;
	}
	else if (ViewportAspectRatio > CameraAspectRatio)
	{
		// Left and right black bars
		float VerticalBlackBarWidth = (ViewportSize.X - (CameraAspectRatio * ViewportSize.Y)) / 2.0f;
		ScreenMinX += VerticalBlackBarWidth;
		ScreenMaxX -= VerticalBlackBarWidth;
	}

	// Calculate the viewport bounds
	TopLeftBound = ConvertFromScreenCoordinates(PlayerController, FVector2D(ScreenMinX, ScreenMinY),
		CameraDistance);
	BottomRightBound = ConvertFromScreenCoordinates(PlayerController, FVector2D(ScreenMaxX, ScreenMaxY),
		CameraDistance);

	return true;
}
