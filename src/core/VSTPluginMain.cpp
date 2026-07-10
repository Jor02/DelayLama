#include <Windows.h>
#include <winnt.h>
#include "DelayLamaPlugin.h"
#include "utils/Logger.h"
#include "damsdk/api/DamPlugin.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

using namespace Utils;

namespace DelayLama {
namespace Core {

    // GLOBAL: DELAYLAMA: 0x1000d404
    bool g_bInitFailed = false;

    // GLOBAL: DELAYLAMA: 0x1000c248
    LONG g_cRefCount = 0;

    // GLOBAL: DELAYLAMA: 0x1000b080
    void* g_pfnCustomInit = nullptr;

    // STUB: CRT initialization handler
    int crtInitHandler(HINSTANCE hinst, DWORD reason) {
        // Placeholder - actual implementation would handle CRT initialization
        return 1;
    }

    // FUNCTION DELAYLAMA: 0x10003560
    extern "C" __declspec(dllexport) DamSDK::Api::DamPlugin* __cdecl VSTPluginMain(DamSDK::Api::dispatchFunc hostCallback)
    {
#ifdef DELAYLAMA_DEBUG_CONSOLE
        attachConsole();
#endif

        log("VSTPluginMain called\n");

        // Check host version
        intptr_t hostVersionSupport = hostCallback(nullptr, DamSDK::Api::hostGetApiVersion, 0, 0, nullptr, 0.0f);
        if (hostVersionSupport == 0) {
            return nullptr;
        }

        log("Host version checked\n");

        DelayLamaPlugin* delayLama = new DelayLamaPlugin(hostCallback);
    
        if (g_bInitFailed) {
            delayLama->~DelayLamaPlugin();
            return NULL;
        }

        // Return the DamPlugin
        DamSDK::Api::DamPlugin* plugin = &delayLama->plugin;

        log("Returning DamPlugin poiner to host\n");

        return plugin;
    }
}
}