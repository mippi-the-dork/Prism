#pragma once

#include "CoreMinimal.h"

class UPrismLibraryEntry;
class UTextureGraph;

/** Shared filtering and validation used by both the Prism browser and native automation. */
class PRISMEDITOR_API FPrismLibraryCatalog
{
public:
    static bool IsRegistered(const UPrismLibraryEntry* Entry);
    static bool MatchesSearch(const UPrismLibraryEntry* Entry, const FString& Query);
    static bool IsDuplicateGraph(const UTextureGraph* Graph, const TArray<UPrismLibraryEntry*>& Entries);
    static bool HasValidReplacement(const UPrismLibraryEntry* Entry);
    static void QueryEntries(TArray<UPrismLibraryEntry*>& OutEntries);
};
