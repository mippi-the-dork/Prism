#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PrismLibraryEntryFactory.generated.h"

UCLASS()
class UPrismLibraryEntryFactory : public UFactory
{
    GENERATED_BODY()
public:
    UPrismLibraryEntryFactory();
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
        EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
