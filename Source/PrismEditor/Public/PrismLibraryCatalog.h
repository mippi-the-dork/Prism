#pragma once

#include "CoreMinimal.h"

class UPrismFunction;
class UTextureGraph;

/** Shared filtering and validation used by both the Prism browser and native automation. */
class PRISMEDITOR_API FPrismLibraryCatalog
{
public:
    static bool IsRegistered(const UPrismFunction* Entry);
    static bool MatchesSearch(const UPrismFunction* Entry, const FString& Query);
    static bool IsDuplicateGraph(const UTextureGraph* Graph, const TArray<UPrismFunction*>& Entries);
    static bool HasValidReplacement(const UPrismFunction* Entry);
    static void QueryEntries(TArray<UPrismFunction*>& OutEntries);
};
