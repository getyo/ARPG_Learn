namespace UnrealBuildTool.Rules
{
	using System.Collections.Generic;
	using UnrealBuildTool;
	using System.IO;
	using System;

	public class UNTLink : ModuleRules
	{
		public UNTLink(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Projects",
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"WebSockets",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"Json",
				"JsonUtilities",
				"GraphEditor",
				"RenderCore",
				"RHI",
				"HTTP",
				"UMG",
				"WebBrowser",
				"Networking",
				"Sockets",
				"ToolMenus",
				"ContentBrowserData",
				"PlacementMode",
				"ApplicationCore",
				"AssetRegistry",
				"EditorStyle",
				"LevelEditor"
				
			});
		}
	}
}