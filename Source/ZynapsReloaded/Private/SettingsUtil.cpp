// Copyright (c) 2017 Bytecode Bits

#include "ZynapsReloaded.h"
#include "SettingsUtil.h"

// Log category
DEFINE_LOG_CATEGORY(LogSettingsUtil);

// Returns all the game user settings
UGameUserSettings* USettingsUtil::GetGameUserSettings()
{
	if (GEngine)
	{
		return GEngine->GameUserSettings;
	}
	UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the instance of GEngine"));
	return NULL;
}

// Returns the resolution settings and returns true if everything was fine
bool USettingsUtil::GetResolutionSettings(int32& Width, int32& Height, bool& bFullscreen)
{
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	FIntPoint Resolution = Settings->GetScreenResolution();
	Width = Resolution.X;
	Height = Resolution.Y;

	EWindowMode::Type WindowMode = Settings->GetFullscreenMode();
	if (WindowMode == EWindowMode::Fullscreen || WindowMode == EWindowMode::WindowedFullscreen)
	{
		bFullscreen = true;
	}
	else
	{
		bFullscreen = false;
	}

	return true;
}

// Sets the resolution settings and returns true if everything was fine
bool USettingsUtil::SetResolutionSettings(int32 Width, int32 Height, bool bFullscreen)
{
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	Settings->SetScreenResolution(FIntPoint(Width, Height));
	Settings->SetFullscreenMode(bFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
	return true;
}

// Returns the graphics quality and rendering settings. Returns true if successful
bool USettingsUtil::GetScalabilitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess,
	int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance, int32& Foliage)
{
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	AntiAliasing = Settings->ScalabilityQuality.AntiAliasingQuality;
	Effects = Settings->ScalabilityQuality.EffectsQuality;
	PostProcess = Settings->ScalabilityQuality.PostProcessQuality;
	Resolution = Settings->ScalabilityQuality.ResolutionQuality;
	Shadow = Settings->ScalabilityQuality.ShadowQuality;
	Texture = Settings->ScalabilityQuality.TextureQuality;
	ViewDistance = Settings->ScalabilityQuality.ViewDistanceQuality;
	Foliage = Settings->ScalabilityQuality.FoliageQuality;

	return true;
}

// Sets the graphics quality and rendering settings and returns true on success
bool USettingsUtil::SetScalabilitySettings(int32 AntiAliasing, int32 Effects, int32 PostProcess,
	int32 Resolution, int32 Shadow, int32 Texture, int32 ViewDistance, int32 Foliage)
{
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	Settings->ScalabilityQuality.AntiAliasingQuality = AntiAliasing;
	Settings->ScalabilityQuality.EffectsQuality = Effects;
	Settings->ScalabilityQuality.PostProcessQuality = PostProcess;
	Settings->ScalabilityQuality.ResolutionQuality = Resolution;
	Settings->ScalabilityQuality.ShadowQuality = Shadow;
	Settings->ScalabilityQuality.TextureQuality = Texture;
	Settings->ScalabilityQuality.ViewDistanceQuality = ViewDistance;
	Settings->ScalabilityQuality.FoliageQuality = Foliage;

	return true;
}

// Applies and saves the display user settings
bool USettingsUtil::ApplyAndSaveDisplaySettings()
{
	UGameUserSettings* Settings = USettingsUtil::GetGameUserSettings();
	if (!Settings)
	{
		UE_LOG(LogSettingsUtil, Error, TEXT("Failed to get the game user settings"));
		return false;
	}

	Settings->ConfirmVideoMode();
	Settings->ApplyNonResolutionSettings();
	Settings->SaveSettings();

	return true;
}

// Returns all the game custom settings
UCustomGameConfig* USettingsUtil::GetCustomGameSettings()
{
	// This should be a singleton. Making it a static member does not work as it seems that
	// it is being garbage-collected. A solution may be to have a GameSingleton class.
	return NewObject<UCustomGameConfig>();
}

// Applies and saves all the game custom settings
void USettingsUtil::ApplyAndSaveCustomGameSettings(UCustomGameConfig* CustomGameSettings)
{
	CustomGameSettings->SaveConfig();
}
