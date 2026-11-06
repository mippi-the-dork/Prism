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

#endif
