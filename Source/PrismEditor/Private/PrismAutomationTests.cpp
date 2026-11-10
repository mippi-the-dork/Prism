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
#include "PrismFunction.h"
#include "PrismFunctionFactory.h"
#include "AssetDefinition_PrismFunction.h"
#include "AssetToolsModule.h"
#include "AssetTypeCategories.h"
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismRegionDerivativesTest, "Prism.Algorithms.RegionDerivedChannelsCPU",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismRegionDerivativesTest::RunTest(const FString& Parameters)
{
    // Two disconnected 2x2 islands in a 5x2 mask.
    const TArray<int32> Labels = { 1, 1, 0, 2, 2, 1, 1, 0, 2, 2 };
    TArray<float> Values;
    TestTrue(TEXT("Random grayscale generated"), FPrismRegionData::Generate(
        Labels, 5, 2, EPrismRegionChannel::RandomGrayscale, 42, Values));
    if (Values.Num() != Labels.Num()) return false;
    TestEqual(TEXT("Same region has same random value"), Values[0], Values[6]);
    TestEqual(TEXT("Second region is consistent"), Values[3], Values[9]);
    TestEqual(TEXT("Background is black"), Values[2], 0.f);
    const float First = Values[0];
    TestTrue(TEXT("Different seed generated"), FPrismRegionData::Generate(
        Labels, 5, 2, EPrismRegionChannel::RandomGrayscale, 43, Values));
    TestNotEqual(TEXT("Seed changes random value"), First, Values[0]);
    TestTrue(TEXT("Region index generated"), FPrismRegionData::Generate(
        Labels, 5, 2, EPrismRegionChannel::NormalizedIndex, 0, Values));
    TestEqual(TEXT("First index normalized"), Values[0], 0.5f);
    TestEqual(TEXT("Second index normalized"), Values[4], 1.f);
    TestTrue(TEXT("Local horizontal gradient generated"), FPrismRegionData::Generate(
        Labels, 5, 2, EPrismRegionChannel::LocalGradientX, 0, Values));
    TestTrue(TEXT("Gradient increases inside island"), Values[0] < Values[1]);
    TestEqual(TEXT("Gradient is consistent across rows"), Values[0], Values[5]);
    TArray<int32> Invalid = Labels;
    Invalid[0] = -1;
    TestFalse(TEXT("Invalid labels rejected"), FPrismRegionData::Generate(
        Invalid, 5, 2, EPrismRegionChannel::PositionX, 0, Values));
    return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismRegionStatisticsTest, "Prism.Algorithms.RegionStatisticsCPU",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismRegionStatisticsTest::RunTest(const FString& Parameters)
{
    // Two 2x2 islands in a 5x2 canvas; deterministic, pixel-center centroids.
    const TArray<int32> Labels = {1, 1, 0, 2, 2, 1, 1, 0, 2, 2};
    TArray<FPrismRegionStats> Stats;
    TestTrue(TEXT("Measurements succeed"), FPrismRegionData::Measure(Labels, 5, 2, Stats));
    if (Stats.Num() != 3) return false;
    TestEqual(TEXT("Region one pixel count"), Stats[1].PixelCount, 4);
    TestEqual(TEXT("Region one left"), Stats[1].MinX, 0);
    TestEqual(TEXT("Region one right"), Stats[1].MaxX, 1);
    TestEqual(TEXT("Region two left"), Stats[2].MinX, 3);
    TestEqual(TEXT("Region two right"), Stats[2].MaxX, 4);
    TestEqual(TEXT("Region one centroid X"), Stats[1].CentroidX, 1.0);
    TestEqual(TEXT("Region two centroid X"), Stats[2].CentroidX, 4.0);
    TestEqual(TEXT("Region centroid Y"), Stats[1].CentroidY, 1.0);
    const TArray<int32> Sparse = {1, 0, 3};
    TestFalse(TEXT("Sparse labels rejected"), FPrismRegionData::Measure(Sparse, 3, 1, Stats));
    const TArray<int32> Excessive = {2147483647, 0, 0};
    TestFalse(TEXT("Invalid oversized label rejected"), FPrismRegionData::Measure(Excessive, 3, 1, Stats));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismRegionAdvancedChannelsTest, "Prism.Algorithms.RegionAdvancedChannelsCPU",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismRegionAdvancedChannelsTest::RunTest(const FString& Parameters)
{
    const TArray<int32> Labels = {1, 1, 0, 2, 2, 1, 1, 0, 2, 2};
    TArray<float> V;
    TestTrue(TEXT("Area channel"), FPrismRegionData::Generate(Labels, 5, 2, EPrismRegionChannel::AreaNormalized, 1, V));
    if (V.Num() != 10) return false;
    TestEqual(TEXT("Region area fraction"), V[0], 0.4f);
    TestEqual(TEXT("Area consistent"), V[0], V[6]);
    TestTrue(TEXT("Centroid X"), FPrismRegionData::Generate(Labels, 5, 2, EPrismRegionChannel::CentroidX, 1, V));
    TestEqual(TEXT("First centroid X normalized"), V[0], 0.2f);
    TestEqual(TEXT("Second centroid X normalized"), V[3], 0.8f);
    TestTrue(TEXT("Centroid Y"), FPrismRegionData::Generate(Labels, 5, 2, EPrismRegionChannel::CentroidY, 1, V));
    TestEqual(TEXT("Centroid Y normalized"), V[0], 0.5f);
    TestTrue(TEXT("Region width"), FPrismRegionData::Generate(Labels, 5, 2, EPrismRegionChannel::BoundingBoxWidth, 1, V));
    TestEqual(TEXT("Width fraction"), V[0], 0.4f);
    TestTrue(TEXT("Region height"), FPrismRegionData::Generate(Labels, 5, 2, EPrismRegionChannel::BoundingBoxHeight, 1, V));
    TestEqual(TEXT("Height fraction"), V[0], 1.0f);
    TestEqual(TEXT("Background remains black"), V[2], 0.0f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismRegionEndToEndCPUTest, "Prism.Algorithms.RegionMaskToChannelCPU",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismRegionEndToEndCPUTest::RunTest(const FString& Parameters)
{
    const TArray<uint8> Mask = {1, 0, 1, 0, 1, 0, 1, 0, 1};
    TArray<float> Output;
    int32 Regions = -1;
    TestTrue(TEXT("Four-connected mask evaluated"), FPrismRegionData::GenerateFromMask(
        Mask, 3, 3, false, EPrismRegionChannel::NormalizedIndex, 7, Output, Regions));
    TestEqual(TEXT("Four-connected checkerboard has five regions"), Regions, 5);
    if (Output.Num() != 9) return false;
    TestEqual(TEXT("First region index"), Output[0], 0.2f);
    TestEqual(TEXT("Last region index"), Output[8], 1.0f);
    TestEqual(TEXT("Background remains zero"), Output[1], 0.0f);
    TestTrue(TEXT("Eight-connected mask evaluated"), FPrismRegionData::GenerateFromMask(
        Mask, 3, 3, true, EPrismRegionChannel::NormalizedIndex, 7, Output, Regions));
    TestEqual(TEXT("Eight-connected checkerboard is one region"), Regions, 1);
    TestEqual(TEXT("Merged component gets full label"), Output[0], 1.0f);
    TestFalse(TEXT("Invalid mask dimensions rejected"), FPrismRegionData::GenerateFromMask(
        Mask, 4, 3, false, EPrismRegionChannel::NormalizedIndex, 7, Output, Regions));
    TestEqual(TEXT("Failed pipeline clears region count"), Regions, 0);
    TestEqual(TEXT("Failed pipeline clears values"), Output.Num(), 0);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismFunctionAssetIdentityTest, "Prism.Library.FunctionAssetIdentity",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismFunctionAssetIdentityTest::RunTest(const FString& Parameters)
{
    const UPrismFunctionFactory* Factory = GetDefault<UPrismFunctionFactory>();
    const UAssetDefinition_PrismFunction* Definition = GetDefault<UAssetDefinition_PrismFunction>();
    TestNotNull(TEXT("Prism Function factory"), Factory);
    TestNotNull(TEXT("Prism Function definition"), Definition);
    if (!Factory || !Definition) return false;
    // UFactory::bCreateNew is protected in UE 5.8; test actual creation through the public factory API.
    UPrismFunctionFactory* MutableFactory = NewObject<UPrismFunctionFactory>(GetTransientPackage());
    TestNotNull(TEXT("Prism Function factory instance"), MutableFactory);
    if (!MutableFactory) return false;
    UObject* Created = MutableFactory->FactoryCreateNew(
        UPrismFunction::StaticClass(), GetTransientPackage(),
        MakeUniqueObjectName(GetTransientPackage(), UPrismFunction::StaticClass(), TEXT("PrismFunctionTest")),
        RF_Transient, nullptr, nullptr);
    TestNotNull(TEXT("Factory creates a Prism Function asset"), Created);
    TestTrue(TEXT("Factory creates the correct asset class"), Created && Created->IsA<UPrismFunction>());
    TestEqual(TEXT("Factory supports Prism Function"), Factory->GetSupportedClass(), UPrismFunction::StaticClass());
    TestEqual(TEXT("Asset label is Prism Function"), Definition->GetAssetDisplayName().ToString(), FString(TEXT("Prism Function")));
    TestEqual(TEXT("Asset identity class"), Definition->GetAssetClass().Get(), static_cast<UClass*>(UPrismFunction::StaticClass()));
    TestEqual(TEXT("Texture category factory"), Factory->GetMenuCategories(), static_cast<uint32>(EAssetTypeCategories::Textures));
    TestEqual(TEXT("One nested asset category"), static_cast<int32>(Definition->GetAssetCategories().Num()), 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismEntryDefaultsTest, "Prism.Library.EntryDefaults",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismEntryDefaultsTest::RunTest(const FString& Parameters)
{
    UPrismFunction* Entry = NewObject<UPrismFunction>(GetTransientPackage());
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
    UPrismFunction* Entry = NewObject<UPrismFunction>(GetTransientPackage());
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
    UPrismFunction* Entry = NewObject<UPrismFunction>(GetTransientPackage());
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
    UPrismFunction* Entry = NewObject<UPrismFunction>(GetTransientPackage());
    Entry->Graph = Graph;
    const TArray<UPrismFunction*> Entries = { Entry };
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
    UPrismFunction* Original = NewObject<UPrismFunction>(GetTransientPackage());
    UPrismFunction* Replacement = NewObject<UPrismFunction>(GetTransientPackage());
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

#if WITH_DEV_AUTOMATION_TESTS
// Analytical validation of the triangular lattice used by the Pattern Generator's
// hexagon mode. These assertions do NOT replace a shader-pixel readback test.
namespace PrismHexReference
{
    struct FSample { FVector2D Delta; FIntPoint Cell; };
    static FSample Sample(const FVector2D& UV, int32 Columns, int32 Rows)
    {
        const int32 EvenRows = Rows + (Rows & 1);
        const double Step = 0.8660254037844386;
        const FVector2D P(UV.X * Columns, UV.Y * EvenRows * Step);
        const int32 FirstRow = FMath::FloorToInt(P.Y / Step);
        double BestSquared = TNumericLimits<double>::Max();
        FSample Best{};
        for (int32 J=FirstRow-1; J<=FirstRow+1; ++J)
        {
            const int32 Shift = ((J%2)+2)%2;
            const int32 BaseCol=FMath::FloorToInt(P.X - 0.5*Shift);
            for(int32 I=BaseCol-1; I<=BaseCol+1; ++I)
            {
                const FVector2D Delta=P-FVector2D(I+0.5*Shift+0.5,(J+0.5)*Step);
                if (Delta.SizeSquared() < BestSquared)
                {
                    BestSquared=Delta.SizeSquared();
                    Best={Delta,FIntPoint(I,J)};
                }
            }
        }
        return Best;
    }
    static double HexBoundary(const FVector2D& D)
    {
        const double X=FMath::Abs(D.X),Y=FMath::Abs(D.Y);
        return FMath::Max(X,0.5*X+0.8660254037844386*Y);
    }
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismHexLatticeTest, "Prism.Patterns.Hexagon.Geometry", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismHexLatticeTest::RunTest(const FString& Parameters)
{
    // A true honeycomb must have displaced alternating rows and shared edges.
    const int32 C=8,R=8;
    const double RowStep=0.8660254037844386;
    const auto A=PrismHexReference::Sample(FVector2D(0.5/C,0.5/R),C,R);
    const auto B=PrismHexReference::Sample(FVector2D(1.0/C,1.5/R),C,R);
    TestTrue(TEXT("Even-row hex center"),A.Delta.SizeSquared()<1e-8);
    TestTrue(TEXT("Odd-row hex center staggered half a cell"),B.Delta.SizeSquared()<1e-8);
    const auto Mid=PrismHexReference::Sample(FVector2D(0.75/C,1.0/R),C,R);
    TestTrue(TEXT("Neighbor seam is hex boundary"),FMath::IsNearlyEqual(PrismHexReference::HexBoundary(Mid.Delta),0.5,1e-6));
    // Check that *all* 64 rows and columns in a dense representative sample
    // lie inside the nearest hex at full fill (no square-cell gaps).
    bool bCovered=true;
    for(int32 Y=0;Y<64;++Y) for(int32 X=0;X<64;++X)
    {
        const auto P=PrismHexReference::Sample(FVector2D((X+0.37)/64.0,(Y+0.23)/64.0),C,R);
        bCovered &= PrismHexReference::HexBoundary(P.Delta)<=0.500001;
    }
    TestTrue(TEXT("Fully filled regular hex grid covers the plane"),bCovered);
    for(int32 Y=0;Y<8;++Y)
    {
        const double V=(Y+0.41)/8;
        const auto Left=PrismHexReference::Sample(FVector2D(0.001,V),C,R);
        const auto Right=PrismHexReference::Sample(FVector2D(1.001,V),C,R);
        TestTrue(TEXT("Horizontal tile seam repeats"),Left.Delta.Equals(Right.Delta,0.000001));
        const auto Top=PrismHexReference::Sample(FVector2D(V,0.001),C,R);
        const auto Bottom=PrismHexReference::Sample(FVector2D(V,1.001),C,R);
        TestTrue(TEXT("Vertical tile seam repeats"),Top.Delta.Equals(Bottom.Delta,0.000001));
    }
    // Odd row values should be rounded to even for seamless stagger parity.
    const auto OddTop=PrismHexReference::Sample(FVector2D(0.34,0.001),7,7);
    const auto OddBottom=PrismHexReference::Sample(FVector2D(0.34,1.001),7,7);
    TestTrue(TEXT("Odd row count also tiles by using next even row count"),OddTop.Delta.Equals(OddBottom.Delta,0.000001));
    TestTrue(TEXT("Row step remains regular"),FMath::IsNearlyEqual(RowStep, FMath::Sqrt(3.0)/2.0,1e-8));
    return true;
}
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPrismSamplerMapsTest, "Prism.Patterns.Sampler.ModulationPins", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FPrismSamplerMapsTest::RunTest(const FString& Parameters)
{
    const auto* Node=NewObject<UPrismExpression_PatternSampler>();
    TestTrue(TEXT("Sampler signature valid"),Node->GetSignature().IsValid());
    for (const FName Field : {GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternSampler, ScaleMap),GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternSampler, RotationMap),GET_MEMBER_NAME_CHECKED(UPrismExpression_PatternSampler, OffsetMap)})
    {
        const FProperty* Property=Node->GetClass()->FindPropertyByName(Field);
        TestNotNull(TEXT("Modulation map reflected"),Property);
        if(Property)
        {
            TestEqual(TEXT("Modulation map is a TG input"),Property->GetMetaData(TEXT("TGType")),FString(TEXT("TG_Input")));
            TestFalse(TEXT("FTG_Texture not Blueprint-exposed"),Property->HasAnyPropertyFlags(CPF_BlueprintVisible));
        }
    }
    TestEqual(TEXT("Scale map is neutral by default"),Node->ScaleMapInfluence,0.0f);
    TestEqual(TEXT("Rotation map is neutral by default"),Node->RotationMapInfluence,0.0f);
    TestEqual(TEXT("Offset map is neutral by default"),Node->OffsetMapInfluence,0.0f);
    return true;
}
#endif
