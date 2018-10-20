// Copyright (c) 2017 Bytecode Bits

using UnrealBuildTool;
using System.Collections.Generic;

public class ZynapsReloadedTarget : TargetRules
{
	public ZynapsReloadedTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("ZynapsReloaded");
	}
}
