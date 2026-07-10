#include <Windows.h>
#include "damsdk/gui/platform/windows/GDIDrawingContext.h"

using DamSDK::Gui::Platform::Windows::GDIDrawingContext;

namespace DelayLama {
namespace Core {

    // Global variables (need to be defined elsewhere, declared here)
    extern LONG g_cRefCount;
    extern bool g_bInitFailed;
    extern void* g_pfnCustomInit;
    extern int crtInitHandler(HINSTANCE, DWORD);

    // FUNCTION DELAYLAMA: 0x1000a9a7
    extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
        DWORD dwOriginalReason = fdwReason;
        int bCrtInitResult1 = g_cRefCount;

        if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH) {
            if (g_pfnCustomInit != nullptr) {
                // Cast void* to proper function pointer type
                typedef int (*CustomInitFunc)(HINSTANCE, DWORD, LPVOID);
                bCrtInitResult1 = (*(CustomInitFunc)g_pfnCustomInit)(hinstDLL, fdwReason, lpvReserved);
                if (bCrtInitResult1 == 0) {
                    return FALSE;
                }
            }
            bCrtInitResult1 = crtInitHandler(hinstDLL, fdwReason);
            if (bCrtInitResult1 == 0) {
                return FALSE;
            }
        } 
        else if (fdwReason == DLL_PROCESS_DETACH) {
            if (bCrtInitResult1 == 0) {
                return FALSE;
            }
        }

        int bSetModuleResult = GDIDrawingContext::setModuleHandle(hinstDLL);

        if (fdwReason == DLL_PROCESS_ATTACH) {
            if (bSetModuleResult != 0) {
                return bSetModuleResult;
            }
            // Cleanup if setModuleHandle fails
            crtInitHandler(hinstDLL, DLL_PROCESS_DETACH); 
        }

        if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH) {
            return bSetModuleResult;
        }

        int bCrtInitResult2 = crtInitHandler(hinstDLL, fdwReason);
        int bFinalResult = bSetModuleResult;
        if (bCrtInitResult2 == 0) {
            bFinalResult = 0;
        }

        if (bFinalResult != 0) {
            if (g_pfnCustomInit != nullptr) {
                // Cast void* to proper function pointer type
                typedef int (*CustomInitFunc)(HINSTANCE, DWORD, LPVOID);
                return (*(CustomInitFunc)g_pfnCustomInit)(hinstDLL, dwOriginalReason, lpvReserved);
            }
            return bFinalResult;
        }

        return FALSE;
    }
}
}