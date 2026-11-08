#include "Expressions/PrismExpression_NoiseGenerator.h"
#include "PrismNodeEnums.h"
#include "Expressions/PrismExpression_Posterize.h"
#include "Expressions/PrismExpression_HighPass.h"
#include "Expressions/PrismExpression_Sharpen.h"
#include "Expressions/PrismExpression_FloodFill.h"
#include "Expressions/PrismExpression_GradientMap.h"
#include "Expressions/PrismExpression_Curve.h"
#include "Expressions/PrismExpression_Distance.h"
#include "Misc/AutomationTest.h"
#include "PrismLibraryEntry.h"
#include "TextureGraph.h"
#include "PrismLibraryCatalog.h"
#include "Expressions/PrismExpression_MultiPassBlur.h"

#include "Expressions/PrismExpression_SoftThreshold.h"
#include "Expressions/PrismExpression_LevelsRemap.h"
#include "Expressions/PrismExpression_ClampRange.h"
#include "Expressions/PrismExpression_InvertChannels.h"
#include "Expressions/PrismExpression_EdgeOutline.h"
#include "Expressions/PrismExpression_SobelNormal.h"
#include "Expressions/PrismExpression_Emboss.h"
#include "Expressions/PrismExpression_BevelLighting.h"
#include "Expressions/PrismExpression_LocalContrast.h"
#include "Expressions/PrismExpression_BlendModes.h"
#include "Expressions/PrismExpression_GammaCorrection.h"
#include "Expressions/PrismExpression_Morphology.h"
#include "Expressions/PrismExpression_MaskOutline.h"
#include "Expressions/PrismExpression_NormalBlend.h"
#include "Expressions/PrismExpression_NormalNormalize.h"
#include "Expressions/PrismExpression_NormalInvert.h"
#include "Expressions/PrismExpression_DynamicGradient.h"
#include "Expressions/PrismExpression_DirectionalWarp.h"
#include "Expressions/PrismExpression_HeightBlend.h"
#include "Expressions/PrismExpression_TileSeamPreview.h"
#include "Expressions/PrismExpression_CurveAsset.h"
#include "Expressions/PrismExpression_GradientAsset.h"
#include "Expressions/PrismExpression_GradientGenerator.h"
#include "Expressions/PrismExpression_CurveGenerator.h"
#include "PrismImageAlgorithms.h"
#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEntryDefaultsTest, "Prism.Library.EntryDefaults",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEntryDefaultsTest::RunTest(const FString& Parameters)
{
    UPrismLibraryEntry* Entry = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    TestNotNull(TEXT("Prism entry constructed"), Entry);
    if (!Entry) return false;
    TestFalse(TEXT("New entry is unbound"), Entry->HasGraphReference());
    TestFalse(TEXT("New entry is not deprecated"), Entry->bDeprecated);
    TestEqual(TEXT("New entry starts at version 1"), Entry->LibraryVersion, 1);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEntryReferenceTest, "Prism.Library.ReferenceMetadata",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEntryReferenceTest::RunTest(const FString& Parameters)
{
    UPrismLibraryEntry* Entry = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    UTextureGraph* Graph = NewObject<UTextureGraph>(GetTransientPackage());
    TestNotNull(TEXT("Transient graph created"), Graph);
    if (!Entry || !Graph) return false;
    Entry->Graph = Graph;
    TestTrue(TEXT("Entry points at graph"), Entry->HasGraphReference());
    Entry->Graph.Reset();
    TestFalse(TEXT("Clearing graph clears reference"), Entry->HasGraphReference());
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCatalogSearchTest, "Prism.Library.SearchAndCategories",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCatalogSearchTest::RunTest(const FString& Parameters)
{
    UPrismLibraryEntry* Entry = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    UTextureGraph* Graph = NewObject<UTextureGraph>(Entry);
    Entry->Graph = Graph;
    Entry->DisplayName = FText::FromString(TEXT("Stone Grunge"));
    Entry->Category = TEXT("Generators");
    Entry->Subcategory = TEXT("Surface");
    Entry->Description = FText::FromString(TEXT("Procedural rock mask"));
    Entry->Tags = { TEXT("Wear"), TEXT("Stone") };
    TestTrue(TEXT("Search matches title"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("grunge")));
    TestTrue(TEXT("Search matches category"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("generators")));
    TestTrue(TEXT("Search matches subcategory"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("surface")));
    TestTrue(TEXT("Search matches tags"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("wear")));
    TestTrue(TEXT("Multi-term search can match separate fields"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("stone wear")));
    TestFalse(TEXT("Unmatched query is excluded"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("water")));
    Entry->Graph.Reset();
    TestFalse(TEXT("Unregistered entries do not show in search"), FPrismLibraryCatalog::MatchesSearch(Entry, TEXT("")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCatalogDuplicatesTest, "Prism.Library.DuplicateGraphProtection",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCatalogDuplicatesTest::RunTest(const FString& Parameters)
{
    UTextureGraph* Graph = NewObject<UTextureGraph>(GetTransientPackage());
    UTextureGraph* DifferentGraph = NewObject<UTextureGraph>(GetTransientPackage());
    UPrismLibraryEntry* Entry = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    Entry->Graph = Graph;
    const TArray<UPrismLibraryEntry*> Entries = { Entry };
    TestTrue(TEXT("Existing graph recognized"), FPrismLibraryCatalog::IsDuplicateGraph(Graph, Entries));
    TestFalse(TEXT("Different graph allowed"), FPrismLibraryCatalog::IsDuplicateGraph(DifferentGraph, Entries));
    TestFalse(TEXT("Null graph rejected as duplicate"), FPrismLibraryCatalog::IsDuplicateGraph(nullptr, Entries));
    Entry->Graph.Reset();
    TestFalse(TEXT("Cleared registration frees graph"), FPrismLibraryCatalog::IsDuplicateGraph(Graph, Entries));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCatalogReplacementTest, "Prism.Library.DeprecationAndReplacement",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCatalogReplacementTest::RunTest(const FString& Parameters)
{
    UPrismLibraryEntry* Original = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    UPrismLibraryEntry* Replacement = NewObject<UPrismLibraryEntry>(GetTransientPackage());
    TestFalse(TEXT("Normal entry has no replacement"), FPrismLibraryCatalog::HasValidReplacement(Original));
    Original->bDeprecated = true;
    TestFalse(TEXT("Deprecated without replacement is unresolved"), FPrismLibraryCatalog::HasValidReplacement(Original));
    Original->Replacement = Original;
    TestFalse(TEXT("Self-replacement is invalid"), FPrismLibraryCatalog::HasValidReplacement(Original));
    Original->Replacement = Replacement;
    TestTrue(TEXT("Distinct replacement recognized"), FPrismLibraryCatalog::HasValidReplacement(Original));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismMultiPassBlurRegistrationTest, "Prism.Nodes.MultiPassBlurRegistration",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismMultiPassBlurRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_MultiPassBlur* Node = NewObject<UPrismExpression_MultiPassBlur>(GetTransientPackage());
    TestNotNull(TEXT("Custom TextureGraph expression constructed"), Node);
    if (!Node) return false;
    TestEqual(TEXT("Default passes"), Node->Passes, 2);
    TestEqual(TEXT("Default radius"), Node->Radius, 4);
    TestEqual(TEXT("Categorized as Filter"), Node->GetCategory(), TG_Category::Filter);
    TestTrue(TEXT("TextureGraph signature built from reflected pins"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismDistanceRegistrationTest, "Prism.Nodes.Distance.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismDistanceRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Distance* Node = NewObject<UPrismExpression_Distance>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCurveRegistrationTest, "Prism.Nodes.Curve.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCurveRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Curve* Node = NewObject<UPrismExpression_Curve>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismGradientMapRegistrationTest, "Prism.Nodes.GradientMap.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismGradientMapRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_GradientMap* Node = NewObject<UPrismExpression_GradientMap>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismFloodFillRegistrationTest, "Prism.Nodes.FloodFill.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismFloodFillRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_FloodFill* Node = NewObject<UPrismExpression_FloodFill>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismSharpenRegistrationTest, "Prism.Nodes.Sharpen.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismSharpenRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Sharpen* Node = NewObject<UPrismExpression_Sharpen>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismHighPassRegistrationTest, "Prism.Nodes.HighPass.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismHighPassRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_HighPass* Node = NewObject<UPrismExpression_HighPass>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismPosterizeRegistrationTest, "Prism.Nodes.Posterize.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismPosterizeRegistrationTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Posterize* Node = NewObject<UPrismExpression_Posterize>(GetTransientPackage());
    TestNotNull(TEXT("Node exists"), Node);
    if (!Node) return false;
    TestTrue(TEXT("Signature valid"), Node->GetSignature().IsValid());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismSoftThresholdNodeTest, "Prism.Nodes.SoftThreshold.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismSoftThresholdNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_SoftThreshold* Node = NewObject<UPrismExpression_SoftThreshold>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismLevelsRemapNodeTest, "Prism.Nodes.LevelsRemap.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismLevelsRemapNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_LevelsRemap* Node = NewObject<UPrismExpression_LevelsRemap>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismClampRangeNodeTest, "Prism.Nodes.ClampRange.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismClampRangeNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_ClampRange* Node = NewObject<UPrismExpression_ClampRange>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismInvertChannelsNodeTest, "Prism.Nodes.InvertChannels.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismInvertChannelsNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_InvertChannels* Node = NewObject<UPrismExpression_InvertChannels>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEdgeOutlineNodeTest, "Prism.Nodes.EdgeOutline.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEdgeOutlineNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_EdgeOutline* Node = NewObject<UPrismExpression_EdgeOutline>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismSobelNormalNodeTest, "Prism.Nodes.SobelNormal.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismSobelNormalNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_SobelNormal* Node = NewObject<UPrismExpression_SobelNormal>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEmbossNodeTest, "Prism.Nodes.Emboss.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEmbossNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Emboss* Node = NewObject<UPrismExpression_Emboss>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismBevelLightingNodeTest, "Prism.Nodes.BevelLighting.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismBevelLightingNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_BevelLighting* Node = NewObject<UPrismExpression_BevelLighting>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismLocalContrastNodeTest, "Prism.Nodes.LocalContrast.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismLocalContrastNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_LocalContrast* Node = NewObject<UPrismExpression_LocalContrast>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismBlendModesNodeTest, "Prism.Nodes.BlendModes.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismBlendModesNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_BlendModes* Node = NewObject<UPrismExpression_BlendModes>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismGammaCorrectionNodeTest, "Prism.Nodes.GammaCorrection.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismGammaCorrectionNodeTest::RunTest(const FString& Parameters)
{
    UPrismExpression_GammaCorrection* Node = NewObject<UPrismExpression_GammaCorrection>(GetTransientPackage());
    TestNotNull(TEXT("Node created"), Node);
    if (!Node) return false;
    TestTrue(TEXT("TextureGraph expression signature valid"), Node->GetSignature().IsValid());
    TestEqual(TEXT("Expected category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismMorphologyAlpha8Test, "Prism.Nodes.Morphology.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismMorphologyAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_Morphology* Node=NewObject<UPrismExpression_Morphology>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismMaskOutlineAlpha8Test, "Prism.Nodes.MaskOutline.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismMaskOutlineAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_MaskOutline* Node=NewObject<UPrismExpression_MaskOutline>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismNormalBlendAlpha8Test, "Prism.Nodes.NormalBlend.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismNormalBlendAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_NormalBlend* Node=NewObject<UPrismExpression_NormalBlend>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismNormalNormalizeAlpha8Test, "Prism.Nodes.NormalNormalize.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismNormalNormalizeAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_NormalNormalize* Node=NewObject<UPrismExpression_NormalNormalize>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismNormalInvertAlpha8Test, "Prism.Nodes.NormalInvert.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismNormalInvertAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_NormalInvert* Node=NewObject<UPrismExpression_NormalInvert>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismDynamicGradientAlpha8Test, "Prism.Nodes.DynamicGradient.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismDynamicGradientAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_DynamicGradient* Node=NewObject<UPrismExpression_DynamicGradient>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Adjustment);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismDirectionalWarpAlpha8Test, "Prism.Nodes.DirectionalWarp.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismDirectionalWarpAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_DirectionalWarp* Node=NewObject<UPrismExpression_DirectionalWarp>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismHeightBlendAlpha8Test, "Prism.Nodes.HeightBlend.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismHeightBlendAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_HeightBlend* Node=NewObject<UPrismExpression_HeightBlend>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismTileSeamPreviewAlpha8Test, "Prism.Nodes.TileSeamPreview.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismTileSeamPreviewAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_TileSeamPreview* Node=NewObject<UPrismExpression_TileSeamPreview>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Filter);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCurveAssetAlpha8Test, "Prism.Nodes.CurveAsset.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCurveAssetAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_CurveAsset* Node=NewObject<UPrismExpression_CurveAsset>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Adjustment);
    TestFalse(TEXT("Source is not connected initially"), static_cast<bool>(Node->Input));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismGradientAssetAlpha8Test, "Prism.Nodes.GradientAsset.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismGradientAssetAlpha8Test::RunTest(const FString& Parameters)
{
    UPrismExpression_GradientAsset* Node=NewObject<UPrismExpression_GradientAsset>(GetTransientPackage());
    TestNotNull(TEXT("Node instantiated"),Node);
    if (!Node) return false;
    TestTrue(TEXT("Valid signature"),Node->GetSignature().IsValid());
    TestEqual(TEXT("Correct category"), Node->GetCategory(), TG_Category::Adjustment);
    TestFalse(TEXT("Source is not connected initially"), static_cast<bool>(Node->Input));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismRegionLabelsReferenceTest, "Prism.Algorithms.RegionLabelsCPU", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismRegionLabelsReferenceTest::RunTest(const FString& Parameters)
{
    const TArray<uint8> Mask={1,0,1, 0,1,0, 0,0,1};
    TArray<int32> Labels;
    int32 Count=0;
    TestTrue(TEXT("Labeling valid"),FPrismImageAlgorithms::LabelRegions(Mask,3,3,false,Labels,Count));
    TestEqual(TEXT("4-connected islands"),Count,4);
    TestEqual(TEXT("Background is zero"),Labels[1],0);
    TestTrue(TEXT("8-connected valid"),FPrismImageAlgorithms::LabelRegions(Mask,3,3,true,Labels,Count));
    TestEqual(TEXT("Diagonal foreground forms one island"),Count,1);
    TestFalse(TEXT("Invalid dimensions rejected"),FPrismImageAlgorithms::LabelRegions(Mask,4,3,false,Labels,Count));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismDistanceReferenceTest, "Prism.Algorithms.SignedDistanceCPU", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismDistanceReferenceTest::RunTest(const FString& Parameters)
{
    const TArray<uint8> Mask={0,0,0, 0,1,0, 0,0,0};
    TArray<float> Distances;
    TestTrue(TEXT("Distance valid"), FPrismImageAlgorithms::SignedEuclideanDistance(Mask,3,3,Distances));
    TestTrue(TEXT("Center distance one"),FMath::IsNearlyEqual(Distances[4],1.0f,0.001f));
    TestTrue(TEXT("Edge distance minus one"),FMath::IsNearlyEqual(Distances[1],-1.0f,0.001f));
    TestTrue(TEXT("Corner distance minus sqrt two"),FMath::IsNearlyEqual(Distances[0],-FMath::Sqrt(2.0f),0.001f));
    TestFalse(TEXT("Reject mismatched buffer"),FPrismImageAlgorithms::SignedEuclideanDistance(Mask,2,2,Distances));
    return true;
}
#endif

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismGradientGeneratorTest, "Prism.Nodes.GradientGenerator.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismGradientGeneratorTest::RunTest(const FString& Parameters)
{
    UPrismExpression_GradientGenerator* Node = NewObject<UPrismExpression_GradientGenerator>(GetTransientPackage());
    TestNotNull(TEXT("Gradient generator created"), Node);
    if (!Node) return false;
    TestEqual(TEXT("Generator category"), Node->GetCategory(), TG_Category::Input);
    TestTrue(TEXT("Valid output signature"), Node->GetSignature().IsValid());
    TestNull(TEXT("Curve is optional"), Node->Curve.Get());
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCurveGeneratorTest, "Prism.Nodes.CurveGenerator.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCurveGeneratorTest::RunTest(const FString& Parameters)
{
    UPrismExpression_CurveGenerator* Node = NewObject<UPrismExpression_CurveGenerator>(GetTransientPackage());
    TestNotNull(TEXT("Curve generator created"), Node);
    if (!Node) return false;
    TestEqual(TEXT("Generator category"), Node->GetCategory(), TG_Category::Input);
    TestTrue(TEXT("Valid output signature"), Node->GetSignature().IsValid());
    TestNull(TEXT("Curve is optional"), Node->Curve.Get());
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEmbeddedCurveTest, "Prism.Nodes.Curve.EmbeddedCurve", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEmbeddedCurveTest::RunTest(const FString& Parameters)
{
    UPrismExpression_Curve* Node = NewObject<UPrismExpression_Curve>(GetTransientPackage());
    TestNotNull(TEXT("Curve node created"), Node);
    if (!Node) return false;
    TestEqual(TEXT("Adjustment category"), Node->GetCategory(), TG_Category::Adjustment);
    const FRichCurve* Curve = Node->Curve.GetRichCurveConst();
    TestNotNull(TEXT("Editable curve exists"), Curve);
    if (Curve)
    {
        TestEqual(TEXT("Identity curve start"), Curve->Eval(0.0f), 0.0f);
        TestEqual(TEXT("Identity curve end"), Curve->Eval(1.0f), 1.0f);
    }
    return true;
}
#endif

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEnumUXTest, "Prism.UX.EnumDropdowns", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEnumUXTest::RunTest(const FString& Parameters)
{
    TestNotNull(TEXT("Blend mode enum reflected"), StaticEnum<EPrismBlendMode>());
    TestNotNull(TEXT("Outline mode enum reflected"), StaticEnum<EPrismOutlineMode>());
    TestNotNull(TEXT("Gradient direction enum reflected"), StaticEnum<EPrismGradientDirection>());
    TestNotNull(TEXT("Noise type enum reflected"), StaticEnum<EPrismNoiseType>());
    const auto* Blend = NewObject<UPrismExpression_BlendModes>();
    const auto* Outline = NewObject<UPrismExpression_MaskOutline>();
    const auto* Curve = NewObject<UPrismExpression_CurveGenerator>();
    const auto* Gradient = NewObject<UPrismExpression_GradientGenerator>();
    TestTrue(TEXT("Blend defaults to Normal"), Blend->Mode == EPrismBlendMode::Normal);
    TestTrue(TEXT("Outline defaults to Centered"), Outline->Mode == EPrismOutlineMode::Centered);
    TestTrue(TEXT("Curve generator defaults to Horizontal"), Curve->Direction == EPrismGradientDirection::Horizontal);
    TestTrue(TEXT("Gradient generator defaults to Horizontal"), Gradient->Direction == EPrismGradientDirection::Horizontal);
    TestTrue(TEXT("Blend mode property is reflected enum"), CastField<FEnumProperty>(Blend->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_BlendModes, Mode))) != nullptr);
    TestTrue(TEXT("Outline property is reflected enum"), CastField<FEnumProperty>(Outline->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_MaskOutline, Mode))) != nullptr);
    TestTrue(TEXT("Direction property is reflected enum"), CastField<FEnumProperty>(Gradient->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_GradientGenerator, Direction))) != nullptr);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismNoiseRegistrationTest, "Prism.Nodes.FractalNoise.Registration", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismNoiseRegistrationTest::RunTest(const FString& Parameters)
{
    const auto* Node = NewObject<UPrismExpression_NoiseGenerator>();
    TestTrue(TEXT("Generator defaults to fractal noise"), Node->NoiseType == EPrismNoiseType::Fractal);
    TestEqual(TEXT("Default octaves"), Node->Octaves, 5);
    TestEqual(TEXT("Category Input"), Node->GetCategory(), TG_Category::Input);
    TestTrue(TEXT("Node signature valid"), Node->GetSignature().IsValid());
    return true;
}
#endif

#if WITH_DEV_AUTOMATION_TESTS
#include "Expressions/PrismExpression_PatternGenerator.h"
#include "Expressions/PrismExpression_PatternSampler.h"
#include "Expressions/PrismExpression_CellData.h"
#include "Expressions/PrismExpression_StripeWeave.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismPatternGeneratorTest,"Prism.Patterns.Generator",EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismPatternGeneratorTest::RunTest(const FString& Parameters)
{
    auto* Node=NewObject<UPrismExpression_PatternGenerator>();
    TestEqual(TEXT("Generator category"),Node->GetCategory(),TG_Category::Input);
    TestTrue(TEXT("Signature valid"),Node->GetSignature().IsValid());
    TestTrue(TEXT("Shape is dropdown"),CastField<FEnumProperty>(Node->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternGenerator,Shape)))!=nullptr);
    TestEqual(TEXT("Columns default"),Node->Columns,8);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismPatternSamplerTest,"Prism.Patterns.Sampler",EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismPatternSamplerTest::RunTest(const FString& Parameters)
{
    auto* Node=NewObject<UPrismExpression_PatternSampler>();
    TestTrue(TEXT("Signature valid"),Node->GetSignature().IsValid());
    TestFalse(TEXT("Pattern input unbound"),static_cast<bool>(Node->Pattern));
    TestFalse(TEXT("Distribution optional"),static_cast<bool>(Node->Distribution));
    // FTG_Texture is a native TextureGraph pin type, not a Blueprint-exposed property.
    // BlueprintReadWrite on either property is rejected by Unreal Header Tool.
    const FProperty* PatternProperty = Node->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternSampler, Pattern));
    const FProperty* DistributionProperty = Node->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternSampler, Distribution));
    TestNotNull(TEXT("Pattern property reflected"), PatternProperty);
    TestNotNull(TEXT("Distribution property reflected"), DistributionProperty);
    if (PatternProperty && DistributionProperty)
    {
        TestFalse(TEXT("Pattern must not be Blueprint-exposed"), PatternProperty->HasAnyPropertyFlags(CPF_BlueprintVisible));
        TestFalse(TEXT("Distribution must not be Blueprint-exposed"), DistributionProperty->HasAnyPropertyFlags(CPF_BlueprintVisible));
        TestEqual(TEXT("Pattern is a TextureGraph input"), PatternProperty->GetMetaData(TEXT("TGType")), FString(TEXT("TG_Input")));
        TestEqual(TEXT("Distribution is a TextureGraph input"), DistributionProperty->GetMetaData(TEXT("TGType")), FString(TEXT("TG_Input")));
    }
    TestTrue(TEXT("Rotation normalized"),Node->RandomRotation>=0.0f && Node->RandomRotation<=1.0f);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismCellDataTest,"Prism.Patterns.CellData",EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismCellDataTest::RunTest(const FString& Parameters)
{
    auto* Node=NewObject<UPrismExpression_CellData>();
    TestTrue(TEXT("Signature valid"),Node->GetSignature().IsValid());
    TestTrue(TEXT("Cell data enum"),CastField<FEnumProperty>(Node->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_CellData,Mode)))!=nullptr);
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismStripeWeaveTest,"Prism.Patterns.StripeWeave",EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismStripeWeaveTest::RunTest(const FString& Parameters)
{
    auto* Node=NewObject<UPrismExpression_StripeWeave>();
    TestTrue(TEXT("Signature valid"),Node->GetSignature().IsValid());
    TestTrue(TEXT("Mode dropdown"),CastField<FEnumProperty>(Node->GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UPrismExpression_StripeWeave,Mode)))!=nullptr);
    return true;
}
#endif
