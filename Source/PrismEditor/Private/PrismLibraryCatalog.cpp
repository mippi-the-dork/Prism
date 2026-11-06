#include "PrismLibraryCatalog.h"
#include "PrismLibraryEntry.h"
#include "TextureGraph.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Modules/ModuleManager.h"

bool FPrismLibraryCatalog::IsRegistered(const UPrismLibraryEntry* Entry)
{
    return IsValid(Entry) && !Entry->Graph.IsNull();
}

bool FPrismLibraryCatalog::MatchesSearch(const UPrismLibraryEntry* Entry, const FString& Query)
{
    if (!IsRegistered(Entry)) return false;
    const FString Normalized = Query.TrimStartAndEnd();
    if (Normalized.IsEmpty()) return true;
    FString Haystack = Entry->GetName() + TEXT(" ") + Entry->DisplayName.ToString() + TEXT(" ")
        + Entry->Description.ToString() + TEXT(" ") + Entry->Category.ToString() + TEXT(" ")
        + Entry->Subcategory.ToString() + TEXT(" ") + Entry->Graph.ToSoftObjectPath().ToString();
    for (const FName& SearchTag : Entry->Tags)
    {
        Haystack += TEXT(" ") + SearchTag.ToString();
    }
    // Every search term must match, but terms can appear in different metadata fields.
    TArray<FString> Terms;
    Normalized.ParseIntoArrayWS(Terms);
    for (const FString& Term : Terms)
    {
        if (!Haystack.Contains(Term, ESearchCase::IgnoreCase)) return false;
    }
    return true;
}

bool FPrismLibraryCatalog::IsDuplicateGraph(const UTextureGraph* Graph, const TArray<UPrismLibraryEntry*>& Entries)
{
    if (!IsValid(Graph)) return false;
    const FSoftObjectPath GraphPath(Graph);
    for (const UPrismLibraryEntry* Entry : Entries)
    {
        if (IsRegistered(Entry) && Entry->Graph.ToSoftObjectPath() == GraphPath) return true;
    }
    return false;
}

bool FPrismLibraryCatalog::HasValidReplacement(const UPrismLibraryEntry* Entry)
{
    return IsValid(Entry) && Entry->bDeprecated && !Entry->Replacement.IsNull()
        && Entry->Replacement.ToSoftObjectPath() != FSoftObjectPath(Entry);
}

void FPrismLibraryCatalog::QueryEntries(TArray<UPrismLibraryEntry*>& OutEntries)
{
    OutEntries.Reset();
    FAssetRegistryModule& Registry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    FARFilter Filter;
    Filter.ClassPaths.Add(UPrismLibraryEntry::StaticClass()->GetClassPathName());
    Filter.bRecursiveClasses = true;
    TArray<FAssetData> Assets;
    Registry.Get().GetAssets(Filter, Assets);
    for (const FAssetData& Asset : Assets)
    {
        UPrismLibraryEntry* Entry = Cast<UPrismLibraryEntry>(Asset.GetAsset());
        if (IsRegistered(Entry)) OutEntries.Add(Entry);
    }
}
