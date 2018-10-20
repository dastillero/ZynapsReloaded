// Copyright (c) 2017 Bytecode Bits

using UnrealBuildTool;
using System.Collections.Generic;

public class ZynapsReloadedEditorTarget : TargetRules
{
	public ZynapsReloadedEditorTarget(TargetInfo Target): base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("ZynapsReloaded");
	}
}
