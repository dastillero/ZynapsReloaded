// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GraphicsUtil.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogGraphicsUtil, Log, All);

/**
 * Enum defining several widely used aspect ratios.
 */
UENUM(BlueprintType)
enum class EAspectRatio : uint8
{
	// Any aspect ratio
	AR_Any = 0,
	// 5:4 (1.25)
	AR_5_4 = 1,
	// 4:3 (1.33)
	AR_4_3 = 2,
	// 16:10 (1.60)
	AR_16_10 = 3,
	// 16:9 (1.77)
	AR_16_9 = 4
};

/**
 * Struct which stores the information of a screen resolution.
 */
USTRUCT(BlueprintType)
struct FDisplayAdapterResolution
{
	GENERATED_USTRUCT_BODY()

	// Width
	UPROPERTY(BlueprintReadOnly, Category = Utility)
	int32 Width;

	// Height
	UPROPERTY(BlueprintReadOnly, Category = Utility)
	int32 Height;

	// Refresh rate. It is populated only when querying the available display adapter resolutions.
	UPROPERTY(BlueprintReadOnly, Category = Utility)
	int32 RefreshRate;

	// Calculated aspect ratio (Width / Height)
	UPROPERTY(BlueprintReadOnly, Category = Utility)
	float AspectRatio;

	// Default constructor
	FDisplayAdapterResolution()
	{
		Width = Height = RefreshRate = 0;
		AspectRatio = 0.0f;
	}

	// Constructor with parameters. Aspect ratio is automatically calculated
	FDisplayAdapterResolution(int32 NewWidth, int32 NewHeight, int32 NewRefreshRate = 0)
	{
		Width = NewWidth;
		Height = NewHeight;
		RefreshRate = NewRefreshRate;

		// Calculate the aspect ratio
		float TempAspectRatio = (float)Width / (float)Height;
		AspectRatio = ((int32)(TempAspectRatio * 100.0f)) / 100.0f; // Round to 2 decimals
	}

	// Compares two display adapter resolutions and returns true if both have the same width and height
	FORCEINLINE bool operator==(const FDisplayAdapterResolution& Other) const
	{
		return Width == Other.Width && Height == Other.Height;
	}
};

/**
 * Enum which names scalability settings ranging from 0 to 3
 */
UENUM(BlueprintType)
enum class EScalability4 : uint8
{
	// Low quality
	Low = 0,
	// Medium quality
	Medium = 1,
	// High quality
	High = 2,
	// Ultra quality
	Ultra = 3
};

/**
* Enum which names scalability settings ranging from 0 to 100
*/
UENUM(BlueprintType)
enum class EScalability100 : uint8
{
	// Low quality
	Low = 50,
	// Medium quality
	Medium = 75,
	// High quality
	High = 100
};

/**
 * Structure which store the rendering quality settings.
 */
USTRUCT(BlueprintType)
struct FScalabilitySettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 AntiAliasing;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 Effects;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 PostProcess;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability100 Resolution;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 Shadow;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 Texture;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 ViewDistance;

	UPROPERTY(BlueprintReadWrite, Category = Utility)
	EScalability4 Foliage;

	// Default constructor
	FScalabilitySettings()
	{
		AntiAliasing = Effects = Shadow = EScalability4::Medium;
		PostProcess = ViewDistance = Foliage = EScalability4::Low;
		Texture = EScalability4::Ultra;
		Resolution = EScalability100::High;
	}

	// Constructor with parameters
	FScalabilitySettings(EScalability4 NewAntiAliasing, EScalability4 NewEffects, EScalability4 NewPostProcess,
		EScalability100 NewResolution, EScalability4 NewShadow, EScalability4 NewTexture,
		EScalability4 NewViewDistance, EScalability4 NewFoliage)
	{
		AntiAliasing = NewAntiAliasing;
		Effects = NewEffects;
		PostProcess = NewPostProcess;
		Resolution = NewResolution;
		Shadow = NewShadow;
		Texture = NewTexture;
		ViewDistance = NewViewDistance;
		Foliage = NewFoliage;
	}
};

/**
 * A function library which contains several functions to deal with graphic modes and video configuration.
 */
UCLASS()
class ZYNAPSRELOADED_API UGraphicsUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Returns an array with the resolutions available for the specified aspect ratio. The number of resolutions
	// returned can be limited by the parameter MaxCount. A value of 0 for MaxCount returns all the resolutions
	// with the given aspect ratio. A value for MaxCount > 0 returns the MaxCount highest resolutions found.
	UFUNCTION(BlueprintPure, Category = Utilities)
	static TArray<FDisplayAdapterResolution> GetDisplayAdapterResolutions(EAspectRatio AspectRatio, int32 MaxCount = 0);

	// Tries to find the maximum resolution available with the given aspect ratio. If no resolutions with the aspect
	// ratio are found, it return the maximum resolution available.
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FDisplayAdapterResolution FindRecommendedDisplayAdapterResolution(EAspectRatio PreferredAspectRatio);

	// Returns the display adapter resolution currently selected
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FDisplayAdapterResolution GetDisplayAdapterResolution();

	// Sets a resolution in fullscreen or windowed mode. On success, saves the resolution within the user settings and
	// returns true. Returns false if the resolution could not be set or could not be saved in the user settings.
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetDisplayAdapterResolution(FDisplayAdapterResolution Resolution, bool bFullscreen = true);

	// Returns the current rendering quality settings
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FScalabilitySettings GetScalabilitySettings();

	// Sets the specified rendering quality settings and saves them in the user settings. Returns true on
	// success
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetScalabilitySettings(FScalabilitySettings ScalabilitySettings);

	// Return true if vsync is enabled
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool IsVSyncEnabled();

	// Sets the state of vsync. Returns true on success
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetVSyncEnabled(bool bNewVsyncEnabled);

	// Sets the display adapter resolution and scalability settings stored in the user settings. If the game is 
	// launched for the first time or the graphics settings where not initilized, sets a default resolution 
	// with the given preferred aspect ratio or a different one if the preferred cannot be set. After that, sets
	// the default scalability settings. On success, saves the user settings and returns true. 
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool InitGraphics(EAspectRatio PreferredAspectRatio);

private:

	// Compares two aspect ratios a and b
	static FORCEINLINE bool AreSameAspectRatio(float a, float b)
	{
		return fabs(a - b) < 0.01f;
	}
};
