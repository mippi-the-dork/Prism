using UnrealBuildTool;
public class PrismRender : ModuleRules
{
    public PrismRender(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "TextureGraphEngine", "RHI", "RenderCore" });
        PrivateDependencyModuleNames.AddRange(new[] { "Projects" });
    }
}
