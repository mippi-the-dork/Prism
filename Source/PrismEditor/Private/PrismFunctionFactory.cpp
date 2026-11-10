#include "PrismFunctionFactory.h"
#include "PrismFunction.h"
#include "AssetTypeCategories.h"

UPrismFunctionFactory::UPrismFunctionFactory()
{
    SupportedClass = UPrismFunction::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}
uint32 UPrismFunctionFactory::GetMenuCategories() const
{
    return EAssetTypeCategories::Textures;
}

UObject* UPrismFunctionFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
    EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<UPrismFunction>(InParent, InClass, InName, Flags | RF_Transactional);
}
