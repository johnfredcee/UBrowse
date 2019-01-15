// Some copyright should be here...

using UnrealBuildTool;

public class UBrowse : ModuleRules
{
	public UBrowse(TargetInfo target) 
	{
		
		PublicIncludePaths.AddRange(
			new string[] {
				"UBrowse/Public"
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"UBrowse/Private",
				
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
				"LevelEditor",
                "ClassViewer",
                "GraphEditor",
                "PropertyEditor",
				"CoreUObject", 
                "Engine", 
                "Slate", 
                "SlateCore",		
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
