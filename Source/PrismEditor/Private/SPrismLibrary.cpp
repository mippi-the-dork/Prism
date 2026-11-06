#include "SPrismLibrary.h"
#include "PrismLibraryEntry.h"
#include "PrismLibraryEntryFactory.h"
#include "PrismLibraryCatalog.h"
#include "Logging/LogMacros.h"
#include "TextureGraph.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Misc/PackageName.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Editor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "PrismLibrary"

void SPrismLibrary::Construct(const FArguments& InArgs)
{
    SAssignNew(ListView, SListView<TSharedPtr<FPrismLibraryItem>>)
        .ListItemsSource(&VisibleItems)
        .OnGenerateRow(this, &SPrismLibrary::GenerateRow)
        .SelectionMode(ESelectionMode::Single);
    ChildSlot
    [ SNew(SVerticalBox)
        + SVerticalBox::Slot().AutoHeight().Padding(8,8,8,4)
          [ SNew(STextBlock).Text(LOCTEXT("Header", "Prism | Procedural Function Library"))
            .Font(FAppStyle::GetFontStyle("HeadingExtraSmall")) ]
        + SVerticalBox::Slot().AutoHeight().Padding(8,0,8,3)
          [ SNew(STextBlock).Text_Lambda([this]()
            {
                return FText::Format(LOCTEXT("Results", "{0} registered graphs | {1} visible"),
                    FText::AsNumber(AllItems.Num()), FText::AsNumber(VisibleItems.Num()));
            }).ColorAndOpacity(FSlateColor::UseSubduedForeground()) ]
        + SVerticalBox::Slot().AutoHeight().Padding(8,0,8,6)
          [ SNew(STextBlock).Text(LOCTEXT("Help", "Registered TextureGraphs are reusable subgraphs. Browse selects their assets in the Content Browser; drag the graph into a TextureGraph canvas to insert it."))
            .AutoWrapText(true) ]
        + SVerticalBox::Slot().AutoHeight().Padding(8,0,8,6)
          [ SNew(SHorizontalBox)
            + SHorizontalBox::Slot().FillWidth(1.f)
              [ SNew(SSearchBox).HintText(LOCTEXT("Search", "Search name, category, description, tags"))
                .OnTextChanged(this, &SPrismLibrary::SetSearch) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(4,0,0,0)
              [ SNew(SButton).Text(LOCTEXT("Refresh", "Refresh"))
                .OnClicked_Lambda([this]() { Refresh(); return FReply::Handled(); }) ]
            + SHorizontalBox::Slot().AutoWidth().Padding(4,0,0,0)
              [ SNew(SButton).Text(LOCTEXT("Register", "Register Selected"))
                .ToolTipText(LOCTEXT("RegisterTip", "Create a Prism Library Entry for the TextureGraph selected in the Content Browser."))
                .OnClicked(this, &SPrismLibrary::RegisterSelected) ] ]
        + SVerticalBox::Slot().FillHeight(1.f).Padding(8,0,8,8)
          [ ListView.ToSharedRef() ]
    ];
    Refresh();
    FAssetRegistryModule& Registry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
    AddedHandle = Registry.Get().OnAssetAdded().AddSP(this, &SPrismLibrary::OnRegistryChange);
    RemovedHandle = Registry.Get().OnAssetRemoved().AddSP(this, &SPrismLibrary::OnRegistryChange);
    UpdatedHandle = Registry.Get().OnAssetUpdated().AddSP(this, &SPrismLibrary::OnRegistryChange);
    RenamedHandle = Registry.Get().OnAssetRenamed().AddSP(this, &SPrismLibrary::OnRegistryRename);
}

SPrismLibrary::~SPrismLibrary()
{
    if (FAssetRegistryModule* Registry = FModuleManager::GetModulePtr<FAssetRegistryModule>(TEXT("AssetRegistry")))
    {
        Registry->Get().OnAssetAdded().Remove(AddedHandle);
        Registry->Get().OnAssetRemoved().Remove(RemovedHandle);
        Registry->Get().OnAssetUpdated().Remove(UpdatedHandle);
        Registry->Get().OnAssetRenamed().Remove(RenamedHandle);
    }
}

void SPrismLibrary::OnRegistryChange(const FAssetData& Asset)
{
    if (Asset.AssetClassPath != UPrismLibraryEntry::StaticClass()->GetClassPathName()) return;
    if (!bRefreshPending)
    {
        bRefreshPending = true;
        RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SPrismLibrary::DeferredRefresh));
    }
}

void SPrismLibrary::OnRegistryRename(const FAssetData& Asset, const FString& OldObjectPath)
{
    OnRegistryChange(Asset);
}

EActiveTimerReturnType SPrismLibrary::DeferredRefresh(double CurrentTime, float DeltaTime)
{
    bRefreshPending = false;
    Refresh();
    return EActiveTimerReturnType::Stop;
}

void SPrismLibrary::Refresh()
{
    AllItems.Reset();
    TArray<UPrismLibraryEntry*> Entries;
    FPrismLibraryCatalog::QueryEntries(Entries);
    for (UPrismLibraryEntry* Entry : Entries)
    {
        TSharedPtr<FPrismLibraryItem> Item = MakeShared<FPrismLibraryItem>();
        Item->Entry = TStrongObjectPtr<UPrismLibraryEntry>(Entry);
        Item->Label = Entry->DisplayName.IsEmpty() ? FText::FromString(Entry->GetName()) : Entry->DisplayName;
        Item->Description = Entry->Description;
        Item->Category = Entry->Category;
        AllItems.Add(Item);
    }
    AllItems.Sort([](const TSharedPtr<FPrismLibraryItem>& A, const TSharedPtr<FPrismLibraryItem>& B)
    {
        if (A->Category != B->Category) return A->Category.LexicalLess(B->Category);
        return A->Label.ToString() < B->Label.ToString();
    });
    ApplyFilter();
}

void SPrismLibrary::SetSearch(const FText& Text)
{
    SearchQuery = Text.ToString();
    ApplyFilter();
}
void SPrismLibrary::ApplyFilter()
{
    VisibleItems.Reset();
    for (const TSharedPtr<FPrismLibraryItem>& Item : AllItems)
    {
        if (FPrismLibraryCatalog::MatchesSearch(Item->Entry.Get(), SearchQuery))
            VisibleItems.Add(Item);
    }
    if (ListView.IsValid()) ListView->RequestListRefresh();
}

TSharedRef<ITableRow> SPrismLibrary::GenerateRow(TSharedPtr<FPrismLibraryItem> Item, const TSharedRef<STableViewBase>& Owner)
{
    const UPrismLibraryEntry* Entry = Item->Entry.Get();
    const bool bDeprecated = Entry && Entry->bDeprecated;
    return SNew(STableRow<TSharedPtr<FPrismLibraryItem>>, Owner)
    [ SNew(SHorizontalBox)
      + SHorizontalBox::Slot().FillWidth(1.f).Padding(4)
        [ SNew(SVerticalBox)
          + SVerticalBox::Slot().AutoHeight()
            [ SNew(STextBlock).Text(FText::Format(LOCTEXT("RowName", "{0} / {1}{2}"),
              FText::FromName(Item->Category), Item->Label,
              bDeprecated ? LOCTEXT("Deprecated", " (Deprecated)") : FText::GetEmpty())) ]
          + SVerticalBox::Slot().AutoHeight()
            [ SNew(STextBlock).Text(Item->Description).AutoWrapText(true)
              .ColorAndOpacity(FSlateColor::UseSubduedForeground()) ] ]
      + SHorizontalBox::Slot().AutoWidth().Padding(2)
        [ SNew(SButton).Text(LOCTEXT("Browse", "Browse"))
          .OnClicked(this, &SPrismLibrary::BrowseGraph, Item) ]
      + SHorizontalBox::Slot().AutoWidth().Padding(2)
        [ SNew(SButton).Text(LOCTEXT("Open", "Open"))
          .OnClicked(this, &SPrismLibrary::OpenGraph, Item) ]
      + SHorizontalBox::Slot().AutoWidth().Padding(2)
        [ SNew(SButton).Text(LOCTEXT("Edit", "Edit Entry"))
          .OnClicked(this, &SPrismLibrary::EditEntry, Item) ] ];
}

FReply SPrismLibrary::RegisterSelected()
{
    FContentBrowserModule& CB = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    TArray<FAssetData> Selected;
    CB.Get().GetSelectedAssets(Selected);
    bool bFoundGraph = false;
    for (const FAssetData& Asset : Selected)
    {
        UTextureGraph* Graph = Cast<UTextureGraph>(Asset.GetAsset());
        if (!Graph) continue;
        bFoundGraph = true;
        TArray<UPrismLibraryEntry*> ExistingEntries;
        FPrismLibraryCatalog::QueryEntries(ExistingEntries);
        if (FPrismLibraryCatalog::IsDuplicateGraph(Graph, ExistingEntries))
        {
            UE_LOG(LogTemp, Warning, TEXT("Prism: %s is already registered in the library. No duplicate created."), *Graph->GetPathName());
            return FReply::Handled();
        }
        const FString BaseName = TEXT("PL_") + Graph->GetName();
        FString PackageName, AssetName;
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get();
        AssetTools.CreateUniqueAssetName(TEXT("/Game/PrismLibrary/") + BaseName, TEXT(""), PackageName, AssetName);
        UPrismLibraryEntry* Entry = Cast<UPrismLibraryEntry>(AssetTools.CreateAsset(
            AssetName, FPackageName::GetLongPackagePath(PackageName), UPrismLibraryEntry::StaticClass(),
            NewObject<UPrismLibraryEntryFactory>()));
        if (Entry)
        {
            Entry->Graph = Graph;
            Entry->DisplayName = FText::FromString(Graph->GetName());
            Entry->MarkPackageDirty();
            UE_LOG(LogTemp, Display, TEXT("Prism: Registered %s as %s"), *Graph->GetPathName(), *Entry->GetPathName());
            TArray<UObject*> AssetsToSync;
            AssetsToSync.Add(Entry);
            CB.Get().SyncBrowserToAssets(AssetsToSync);
        }
        Refresh();
        break; // Intentionally single-registration to avoid surprising bulk asset creation.
    }
    if (!bFoundGraph)
    {
        UE_LOG(LogTemp, Warning, TEXT("Prism: Select a TextureGraph asset in the Content Browser before using Register Selected."));
    }
    return FReply::Handled();
}

FReply SPrismLibrary::BrowseGraph(TSharedPtr<FPrismLibraryItem> Item)
{
    if (Item.IsValid() && Item->Entry.IsValid())
    {
        if (UTextureGraph* Graph = Item->Entry->Graph.LoadSynchronous())
        {
            FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"))
                .Get().SyncBrowserToAssets(TArray<UObject*>{Graph});
        }
    }
    return FReply::Handled();
}
FReply SPrismLibrary::OpenGraph(TSharedPtr<FPrismLibraryItem> Item)
{
    if (Item.IsValid() && Item->Entry.IsValid() && GEditor)
        if (UTextureGraph* Graph = Item->Entry->Graph.LoadSynchronous())
            GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Graph);
    return FReply::Handled();
}
FReply SPrismLibrary::EditEntry(TSharedPtr<FPrismLibraryItem> Item)
{
    if (Item.IsValid() && Item->Entry.IsValid() && GEditor)
        GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Item->Entry.Get());
    return FReply::Handled();
}
#undef LOCTEXT_NAMESPACE
