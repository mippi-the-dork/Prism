#include "AssetDefinition_PrismFunction.h"
#include "PrismFunction.h"
#define LOCTEXT_NAMESPACE "AssetDefinition_PrismFunction"
FText UAssetDefinition_PrismFunction::GetAssetDisplayName() const { return LOCTEXT("PrismFunction", "Prism Function"); }
FText UAssetDefinition_PrismFunction::GetAssetDisplayName(const FAssetData& AssetData) const
{
    (void)AssetData;
    return GetAssetDisplayName();
}
FLinearColor UAssetDefinition_PrismFunction::GetAssetColor() const
{
    return FLinearColor::FromSRGBColor(FColor(0x8C, 0x67, 0xD9, 0xFF));
}
TSoftClassPtr<UObject> UAssetDefinition_PrismFunction::GetAssetClass() const { return UPrismFunction::StaticClass(); }
TConstArrayView<FAssetCategoryPath> UAssetDefinition_PrismFunction::GetAssetCategories() const
{
    static const TArray<FAssetCategoryPath> Categories = {
        FAssetCategoryPath(EAssetCategoryPaths::Texture, INVTEXT("Prism"))
    };
    return Categories;
}
#undef LOCTEXT_NAMESPACE
