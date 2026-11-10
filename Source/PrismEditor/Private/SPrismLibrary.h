#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/Views/SListView.h"

class UPrismFunction;
class SSearchBox;

struct FPrismLibraryItem
{
    TStrongObjectPtr<UPrismFunction> Entry;
    FText Label;
    FText Description;
    FName Category;
    FString SearchText;
};

class SPrismLibrary : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SPrismLibrary) {} SLATE_END_ARGS()
    void Construct(const FArguments& InArgs);
    virtual ~SPrismLibrary() override;
private:
    TArray<TSharedPtr<FPrismLibraryItem>> AllItems;
    TArray<TSharedPtr<FPrismLibraryItem>> VisibleItems;
    TSharedPtr<SListView<TSharedPtr<FPrismLibraryItem>>> ListView;
    FString SearchQuery;
    FDelegateHandle AddedHandle;
    FDelegateHandle RemovedHandle;
    FDelegateHandle UpdatedHandle;
    FDelegateHandle RenamedHandle;
    bool bRefreshPending = false;
    void OnRegistryChange(const FAssetData& Asset);
    void OnRegistryRename(const FAssetData& Asset, const FString& OldObjectPath);
    EActiveTimerReturnType DeferredRefresh(double CurrentTime, float DeltaTime);
    void Refresh();
    void ApplyFilter();
    void SetSearch(const FText& InText);
    TSharedRef<ITableRow> GenerateRow(TSharedPtr<FPrismLibraryItem> Item,
        const TSharedRef<STableViewBase>& Owner);
    FReply RegisterSelected();
    FReply OpenGraph(TSharedPtr<FPrismLibraryItem> Item);
    FReply BrowseGraph(TSharedPtr<FPrismLibraryItem> Item);
    FReply EditEntry(TSharedPtr<FPrismLibraryItem> Item);
};
