// CitroenGames 2023

using UnrealBuildTool;
using System.Collections.Generic;

public class StumbleGuysTarget : TargetRules
{
	public StumbleGuysTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "StumbleGuys" } );
	}
}
