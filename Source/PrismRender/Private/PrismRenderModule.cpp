#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"
#include "Misc/Paths.h"
class FPrismRenderModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Prism"));
        if (Plugin.IsValid())
        {
            AddShaderSourceDirectoryMapping(TEXT("/Plugin/Prism"), FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders")));
        }
    }
};
IMPLEMENT_MODULE(FPrismRenderModule, PrismRender)
