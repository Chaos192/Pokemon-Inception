// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PokemonInception : ModuleRules
{
	public PokemonInception(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"UMG",
			"Niagara", 
			"AIModule", 
			"GameplayTasks", 
			"NavigationSystem" ,
			"UMG",
			"Slate",
			"SlateCore",
			"LevelSequence",
			"MovieScene"
		});
	}
}
