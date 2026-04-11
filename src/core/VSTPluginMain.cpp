
extern "C" IMAGE_DOS_HEADER __ImageBase;

using namespace DelayLama::Utils;

namespace DelayLama {
namespace Core {

    bool g_bInitFailed = false;

    // This is what the host calls
    extern "C" __declspec(dllexport) DamSDK::DamPlugin* __cdecl VSTPluginMain(DamSDK::dispatchFunc hostCallback)
    {
        attachConsole();

        log("VSTPluginMain called\n");

        // Check host version
        intptr_t hostVersionSupport = hostCallback(nullptr, DamSDK::hostGetApiVersion, 0, 0, nullptr, 0.0f);
        if (hostVersionSupport == 0) {
            return nullptr;
        }

        log("Host version checked\n");

        DelayLamaPlugin* delayLama = new DelayLamaPlugin(hostCallback);
    
        if (g_bInitFailed) {
            return NULL;
        }

        // Return the DamPlugin
        DamSDK::DamPlugin* plugin = &delayLama->plugin;

        return plugin;
    }
}
}