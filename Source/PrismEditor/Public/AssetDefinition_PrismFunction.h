#pragma once
#include "AssetDefinition_DataAsset.h"
#include "AssetDefinition_PrismFunction.generated.h"

/** Content Browser identity for a reusable Prism TextureGraph function. */
UCLASS()
class PRISMEDITOR_API UAssetDefinition_PrismFunction final : public UAssetDefinition_DataAsset
{
    GENERATED_BODY()
public:
    virtual FText GetAssetDisplayName() const override;
    virtual FText GetAssetDisplayName(const FAssetData& AssetData) const override;
    virtual FLinearColor GetAssetColor() const override;
    virtual TSoftClassPtr<UObject> GetAssetClass() const override;
    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
};
