#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "SPrismLibrary.h"

#define LOCTEXT_NAMESPACE "PrismEditor"

class FPrismEditorModule final : public IModuleInterface
{
public:
    static const FName LibraryTabId;

    virtual void StartupModule() override
    {
        FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LibraryTabId,
            FOnSpawnTab::CreateRaw(this, &FPrismEditorModule::SpawnLibraryTab))
            .SetDisplayName(LOCTEXT("LibraryTab", "Prism Library"))
            .SetTooltipText(LOCTEXT("LibraryDescription", "Browse reusable TextureGraph function library entries."))
            .SetMenuType(ETabSpawnerMenuType::Hidden);
        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
            this, &FPrismEditorModule::RegisterMenus));
    }

    virtual void ShutdownModule() override
    {
        UToolMenus::UnRegisterStartupCallback(this);
        UToolMenus::UnregisterOwner(this);
        FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LibraryTabId);
    }

private:
    TSharedRef<SDockTab> SpawnLibraryTab(const FSpawnTabArgs& Args)
    {
        return SNew(SDockTab).TabRole(ETabRole::NomadTab)
            [ SNew(SPrismLibrary) ];
    }

    void RegisterMenus()
    {
        FToolMenuOwnerScoped Owner(this);
        if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Tools")))
        {
            FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("Prism"));
            Section.AddMenuEntry(TEXT("PrismOpenLibrary"),
                LOCTEXT("OpenLibrary", "Prism Library"),
                LOCTEXT("OpenLibraryTooltip", "Open the Prism procedural TextureGraph library."),
                FSlateIcon(), FUIAction(FExecuteAction::CreateLambda([]()
                {
                    FGlobalTabmanager::Get()->TryInvokeTab(LibraryTabId);
                })));
        }
    }
};
const FName FPrismEditorModule::LibraryTabId(TEXT("PrismLibrary"));
IMPLEMENT_MODULE(FPrismEditorModule, PrismEditor)
#undef LOCTEXT_NAMESPACE
