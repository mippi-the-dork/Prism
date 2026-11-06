#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PrismLibraryEntry.generated.h"

class UTextureGraph;

/** A small, source-control-friendly catalog record for an ordinary reusable TextureGraph asset. */
UCLASS(BlueprintType)
class PRISMEDITOR_API UPrismLibraryEntry : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    TSoftObjectPtr<UTextureGraph> Graph;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    FName Category = TEXT("Functions");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    FName Subcategory = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library", meta=(MultiLine="true"))
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    TArray<FName> Tags;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library", meta=(ClampMin="1", UIMin="1"))
    int32 LibraryVersion = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library")
    bool bDeprecated = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Prism|Library", meta=(EditCondition="bDeprecated"))
    TSoftObjectPtr<UPrismLibraryEntry> Replacement;

    /** Metadata is descriptive; graph inputs and outputs remain authoritative. */
    UFUNCTION(BlueprintPure, Category="Prism|Library")
    bool HasGraphReference() const { return !Graph.IsNull(); }
};
