#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PrismFunction.generated.h"

class UTextureGraph;

/** Metadata describing one reusable TextureGraph. The Prism Library is the collection of Prism Functions. */
UCLASS(BlueprintType)
class PRISMEDITOR_API UPrismFunction : public UDataAsset
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
    TSoftObjectPtr<UPrismFunction> Replacement;

    /** Metadata is descriptive; graph inputs and outputs remain authoritative. */
    UFUNCTION(BlueprintPure, Category="Prism|Library")
    bool HasGraphReference() const { return !Graph.IsNull(); }
};
