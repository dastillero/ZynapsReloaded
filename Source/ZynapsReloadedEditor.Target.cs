// Copyright (c) 2017 Bytecode Bits

using UnrealBuildTool;
using System.Collections.Generic;

public class ZynapsReloadedEditorTarget : TargetRules
{
	public ZynapsReloadedEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.AddRange( new string[] { "ZynapsReloaded" } );
	}
}
