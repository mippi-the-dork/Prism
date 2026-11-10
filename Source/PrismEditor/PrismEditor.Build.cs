using UnrealBuildTool;
public class PrismEditor : ModuleRules
{
    public PrismEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "TextureGraph", "TextureGraphEngine", "PrismRender" });
        PrivateDependencyModuleNames.AddRange(new[] {
            "Slate", "SlateCore", "AssetDefinition", "EngineAssetDefinitions", "UnrealEd", "AssetRegistry", "AssetTools", "ContentBrowser",
            "EditorFramework", "ToolMenus", "Projects", "InputCore", "ApplicationCore",
            "RHI", "RenderCore"
        });
    }
}
