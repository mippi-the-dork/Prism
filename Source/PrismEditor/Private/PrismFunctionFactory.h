#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PrismFunctionFactory.generated.h"

UCLASS()
class UPrismFunctionFactory : public UFactory
{
    GENERATED_BODY()
public:
    UPrismFunctionFactory();
    virtual uint32 GetMenuCategories() const override;
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
        EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
