// Copyright (c) 2017 Bytecode Bits

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomGameConfig.h"
#include "SettingsUtil.generated.h"

// Log category
DECLARE_LOG_CATEGORY_EXTERN(LogSettingsUtil, Log, All);

/**
 * A class with utility methods to manage user settings.
 */
UCLASS()
class ZYNAPSRELOADED_API USettingsUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Returns all the game user settings
	UFUNCTION(BlueprintPure, Category = Utilities)
	static UGameUserSettings* GetGameUserSettings();

	// Returns the resolution settings and returns true if everything was fine
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool GetResolutionSettings(int32& Width, int32& Height, bool& bFullscreen);

	// Sets the resolution settings and returns true if everything was fine
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetResolutionSettings(int32 Width, int32 Height, bool bFullscreen);

	// Returns the graphics quality and rendering settings. Returns true if successful
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool GetScalabilitySettings(int32& AntiAliasing, int32& Effects, int32& PostProcess,
		int32& Resolution, int32& Shadow, int32& Texture, int32& ViewDistance, int32& Foliage);

	// Sets the graphics quality and rendering settings and returns true on success
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetScalabilitySettings(int32 AntiAliasing, int32 Effects, int32 PostProcess,
		int32 Resolution, int32 Shadow, int32 Texture, int32 ViewDistance, int32 Foliage);

	// Returns the status of vsync. Returns true on success
	UFUNCTION(BlueprintPure, Category = Utilities)
	static bool IsVSyncEnabled(bool& Result);

	// Sets the status of vsync. Returns true on success
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool SetVsyncEnabled(bool VSync);

	// Applies and saves the display user settings
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static bool ApplyAndSaveDisplaySettings();

	// Returns all the game custom settings
	UFUNCTION(BlueprintPure, Category = Utilities)
	static UCustomGameConfig* GetCustomGameSettings();

	// Applies and saves all the game custom settings
	UFUNCTION(BlueprintCallable, Category = Utilities)
	static void ApplyAndSaveCustomGameSettings(UCustomGameConfig* CustomGameSettings);
	
};
