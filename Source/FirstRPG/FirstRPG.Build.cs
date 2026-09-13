// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FirstRPG : ModuleRules
{
	public FirstRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore","GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
        //PublicSystemLibraries.Add("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\lib\\x64\\msvcprt.lib");
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
