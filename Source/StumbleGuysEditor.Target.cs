// CitroenGames 2023

using UnrealBuildTool;
using System.Collections.Generic;

public class StumbleGuysEditorTarget : TargetRules
{
	public StumbleGuysEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "StumbleGuys" } );
	}
}
