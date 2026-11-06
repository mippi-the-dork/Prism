#include "PrismLibraryEntryFactory.h"
#include "PrismLibraryEntry.h"

UPrismLibraryEntryFactory::UPrismLibraryEntryFactory()
{
    SupportedClass = UPrismLibraryEntry::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}
UObject* UPrismLibraryEntryFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
    EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<UPrismLibraryEntry>(InParent, InClass, InName, Flags);
}
