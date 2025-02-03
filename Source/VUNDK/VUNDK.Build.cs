using UnrealBuildTool;

public class VUNDK : ModuleRules
{
    public VUNDK(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "VUNDK",
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "EnhancedInput"
            }
        );
        
        // UI Modules
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "UMG"
            }
        );
        
        // AI Modules
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "AIModule"
            }
        );
    }
}