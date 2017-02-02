// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "GraphicsUtil.h"
#include "SettingsUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogGraphicsUtil);

// Returns an array with all the resolutions available for the specified aspect ratio
TArray<FDisplayAdapterResolution> UGraphicsUtil::GetDisplayAdapterResolutions(EAspectRatio AspectRatio)
{
	TArray<FDisplayAdapterResolution> Result;
	FScreenResolutionArray Resolutions;
	if (RHIGetAvailableResolutions(Resolutions, true))
	{
		for (const FScreenResolutionRHI& Resolution : Resolutions)
		{
			FDisplayAdapterResolution Res;
			Res.Width = Resolution.Width;
			Res.Height = Resolution.Height;
			Res.RefreshRate = Resolution.RefreshRate;

			// Calculate the aspect ratio
			float TempAspectRatio = (float)Res.Width / (float)Res.Height;
			Res.AspectRatio = ((int32)(TempAspectRatio * 100.0f)) / 100.0f; // Round to 2 decimals

			UE_LOG(LogGraphicsUtil, VeryVerbose, TEXT("Found resolution %d X %d @ %d with aspect radio %1.2f"),
				Res.Width, Res.Height, Res.RefreshRate, Res.AspectRatio);

			// Add the resolution to the result if it matches the specified one
			bool AddToResult = false;
			switch (AspectRatio)
			{
			case EAspectRatio::AR_Any:
				AddToResult = true;
				break;
			case EAspectRatio::AR_5_4:
				if (AreSameAspectRatio(Res.AspectRatio, 1.25f)) AddToResult = true;
				break;
			case EAspectRatio::AR_4_3:
				if (AreSameAspectRatio(Res.AspectRatio, 1.33f)) AddToResult = true;
				break;
			case EAspectRatio::AR_16_10:
				if (AreSameAspectRatio(Res.AspectRatio, 1.6f)) AddToResult = true;
				break;
			case EAspectRatio::AR_16_9:
				if (AreSameAspectRatio(Res.AspectRatio, 1.77f)) AddToResult = true;
				break;
			}

			if (AddToResult)
			{
				UE_LOG(LogGraphicsUtil, Verbose,
					TEXT("Adding resolution %d X %d @ %d with aspect radio %1.2f to result"),
					Res.Width, Res.Height, Res.RefreshRate, Res.AspectRatio);
				Result.Add(Res);
			}
		}
	}

	return Result;
}

// Sets a resolution in fullscreen or windowed mode. On success, saves the resolution within the user settings and
// returns true. Returns false if the resolution could not be set or could not be saved in the user settings.
bool UGraphicsUtil::SetDisplayAdapterResolution(FDisplayAdapterResolution Resolution, bool bFullscreen)
{
	// Get the user settings
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	// Set the resolution
	EWindowMode::Type WindowMode = bFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed;
	Settings->RequestResolutionChange(Resolution.Width, Resolution.Height, WindowMode, false);

	// Save the resolution in the user settings
	if (!USettingsUtil::SetResolutionSettings(Resolution.Width, Resolution.Height, bFullscreen))
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("The resolution user settings could not be saved"));
		return false;
	}

	// Apply and save the settings
	if (!USettingsUtil::ApplyAndSaveDisplaySettings())
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("Failed to apply and save the resolution user settings"));
	}

	return true;
}

// Returns the current rendering quality settings
FScalabilitySettings UGraphicsUtil::GetScalabilitySettings()
{
	// Get the scalability settings
	int32 AntiAliasing;
	int32 Effects;
	int32 PostProcess;
	int32 Resolution;
	int32 Shadow;
	int32 Texture;
	int32 ViewDistance;
	int32 Foliage;
	if (!USettingsUtil::GetScalabilitySettings(AntiAliasing, Effects, PostProcess, Resolution, Shadow,
		Texture, ViewDistance, Foliage))
	{
		UE_LOG(LogGraphicsUtil, Error,
			TEXT("Failed to get the scalability settings. Returning default scalability values"));
		return FScalabilitySettings();
	}

	// Return the result
	return FScalabilitySettings(
		(EScalability4)AntiAliasing,
		(EScalability4)Effects,
		(EScalability4)PostProcess,
		(EScalability100)Resolution,
		(EScalability4)Shadow,
		(EScalability4)Texture,
		(EScalability4)ViewDistance,
		(EScalability4)Foliage
	);
}

// Sets the specified rendering quality settings and saves them in the user settings. Returns true on
// success
bool UGraphicsUtil::SetScalabilitySettings(FScalabilitySettings ScalabilitySettings)
{
	// Get the user settings
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	// Set the scalability settings
	if (!USettingsUtil::SetScalabilitySettings(
		(int32)ScalabilitySettings.AntiAliasing,
		(int32)ScalabilitySettings.Effects,
		(int32)ScalabilitySettings.PostProcess,
		(int32)ScalabilitySettings.Resolution,
		(int32)ScalabilitySettings.Shadow,
		(int32)ScalabilitySettings.Texture,
		(int32)ScalabilitySettings.ViewDistance,
		(int32)ScalabilitySettings.Foliage))
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("Failed to set the scalability settings"));
		return false;
	}

	// Apply and save the settings
	if (!USettingsUtil::ApplyAndSaveDisplaySettings())
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("Failed to apply and save the scalability settings"));
	}

	return true;
}

// Sets the display adapter resolution and scalability settings stored in the user settings. If the game is 
// launched for the first time or the graphics settings where not initilized, sets a default resolution 
// with the given preferred aspect ratio or a different one if the preferred cannot be set. After that, sets
// the default scalability settings. On success, saves the user settings and returns true. 
bool UGraphicsUtil::InitGraphics(EAspectRatio PreferredAspectRatio)
{
	// Is the game running for the first time?
	UCustomGameConfig* CustomSettings = USettingsUtil::GetCustomGameSettings();
	if (CustomSettings->bGraphicsInitialized)
	{
		// The game is not running for the first time, let the engine handle the configured resolution in
		// the user settings
		UE_LOG(LogGraphicsUtil, Display, TEXT("Using stored user settings for graphics"));
		CustomSettings->bGraphicsInitialized = false;
		USettingsUtil::ApplyAndSaveCustomGameSettings(CustomSettings);
		return true;
	}

	// The graphic setting were not previosly initialized
	UE_LOG(LogGraphicsUtil, Display, TEXT("The graphics settings are not initialized. Choosing a display adapter resolution"));

	// Choose a default screen mode
	TArray<FDisplayAdapterResolution> Resolutions = GetDisplayAdapterResolutions(PreferredAspectRatio);
	if (Resolutions.Num() < 1)
	{
		UE_LOG(LogGraphicsUtil, Warning, 
			TEXT("No display adapter resolutions found with the preferred aspect ratio. Finding alternative resolutions"));
		Resolutions = GetDisplayAdapterResolutions(EAspectRatio::AR_Any);
		if (Resolutions.Num() < 1)
		{
			UE_LOG(LogGraphicsUtil, Error, TEXT("No alternative display adapter resolutions found"));
			return false;
		}
	}
	FDisplayAdapterResolution ChosenResolution = Resolutions[Resolutions.Num() - 1];

	// Try to set the chosen resolution
	UE_LOG(LogGraphicsUtil, Display,
		TEXT("Setting resolution %d X %d @ %dHz - Fullscreen - Aspect Ratio: %1.2f"),
		ChosenResolution.Width, ChosenResolution.Height, ChosenResolution.RefreshRate,
		ChosenResolution.AspectRatio);
	if (!SetDisplayAdapterResolution(ChosenResolution, true))
	{
		UE_LOG(LogGraphicsUtil, Error,
			TEXT("The chosen resolution could not be set: %d X %d @ %dHz - Fullscreen - Aspect Ratio: %1.2f"),
			ChosenResolution.Width, ChosenResolution.Height, ChosenResolution.RefreshRate,
			ChosenResolution.AspectRatio);
		return false;
	}

	// Try to set the default graphics quality and rendering settings
	if (!SetScalabilitySettings(FScalabilitySettings()))
	{
		UE_LOG(LogGraphicsUtil, Error, TEXT("The graphics quality and rendering settings could not be set"));
		return false;
	}

	// The graphics settings have been initialized
	CustomSettings->bGraphicsInitialized = true;
	USettingsUtil::ApplyAndSaveCustomGameSettings(CustomSettings);

	return true;
}
