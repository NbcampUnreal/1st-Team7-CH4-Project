

using UnrealBuildTool;
using System.Collections.Generic;

public class TheSevenTarget : TargetRules
{
	public TheSevenTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "TheSeven" } );
	}
}
