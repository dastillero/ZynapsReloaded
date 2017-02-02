// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GraphicsUtil.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogGraphicsUtil, Log, All);

/**
 * Enum defining several widely used aspect ratios.
 */
UENUM()
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

	// Refresh rate
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
};

/**
 * Enum which names scalability settings ranging from 0 to 3
 */
UENUM()
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
UENUM()
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
		AntiAliasing = Effects = EScalability4::Medium;
		PostProcess = Shadow = ViewDistance = Foliage = EScalability4::Low;
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
 * A function library which contains several functions to deal with graphic modes.
 */
UCLASS()
class ZYNAPSRELOADED_API UGraphicsUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Returns an array with all the resolutions available for the specified aspect ratio
	UFUNCTION(BlueprintPure, Category = Utilities)
	static TArray<FDisplayAdapterResolution> GetDisplayAdapterResolutions(EAspectRatio AspectRatio);

	// Sets a resolution in fullscreen or windowed mode. On success, saves the resolution within the user settings and
	// returns true. Returns false if the resolution could not be set or could not be saved in the user settings.
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetDisplayAdapterResolution(FDisplayAdapterResolution Resolution, bool bFullscreen);

	// Returns the current rendering quality settings
	UFUNCTION(BlueprintPure, Category = Utilities)
	static FScalabilitySettings GetScalabilitySettings();

	// Sets the specified rendering quality settings and saves them in the user settings. Returns true on
	// success
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetScalabilitySettings(FScalabilitySettings ScalabilitySettings);

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
