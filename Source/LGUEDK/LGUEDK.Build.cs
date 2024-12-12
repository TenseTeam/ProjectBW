using UnrealBuildTool;

public class LGUEDK : ModuleRules
{
    public LGUEDK(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                
            }
        );
    }
}