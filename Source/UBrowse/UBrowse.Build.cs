

using UnrealBuildTool;

public class UBrowse : ModuleRules
{
	public UBrowse(ReadOnlyTargetRules Target) : base(Target)
	{
		bUseUnity= false;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"EditorStyle",				
				"Projects",
				"InputCore",
				"UnrealEd",
				"MainFrame",
				"LevelEditor",
                "ClassViewer",
                "GraphEditor",
                "PropertyEditor",
				"CoreUObject", 
                "Engine", 
                "Slate", 
                "SlateCore",	
				"ToolMenus"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
