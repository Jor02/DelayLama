#include <Windows.h>
#include <winnt.h>
#include "DelayLamaPlugin.h"
#include "utils/Logger.h"
#include "damsdk/api/DamPlugin.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

using namespace Utils;

namespace DelayLama {
namespace Core {

    bool g_bInitFailed = false;

    // This is what the host calls
    extern "C" __declspec(dllexport) DamSDK::Api::DamPlugin* __cdecl VSTPluginMain(DamSDK::Api::dispatchFunc hostCallback)
    {
        attachConsole();

        log("VSTPluginMain called\n");

        // Check host version
        intptr_t hostVersionSupport = hostCallback(nullptr, DamSDK::Api::hostGetApiVersion, 0, 0, nullptr, 0.0f);
        if (hostVersionSupport == 0) {
            return nullptr;
        }

        log("Host version checked\n");

        DelayLamaPlugin* delayLama = new DelayLamaPlugin(hostCallback);
    
        if (g_bInitFailed) {
            return NULL;
        }

        // Return the DamPlugin
        DamSDK::Api::DamPlugin* plugin = &delayLama->plugin;

        return plugin;
    }
}
}