#include <Windows.h>
#include <minwindef.h>
#include "platform/GDIDrawingContext.h"

namespace DelayLama {
namespace Core {
    extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
    {
        DWORD dwOriginalReason = fdwReason;
        // int bCrtInitResult1 = g_cRefCount;

        // if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH) {
        //     if (g_pfnCustomInit != nullptr) {
        //         bCrtInitResult1 = (*g_pfnCustomInit)(hinstDLL, fdwReason, lpvReserved);
        //         if (bCrtInitResult1 == 0) {
        //             return FALSE;
        //         }
        //     }
        //     bCrtInitResult1 = crtInitHandler(hinstDLL, fdwReason);
        //     if (bCrtInitResult1 == 0) {
        //         return FALSE;
        //     }
        // } 
        // else if (fdwReason == DLL_PROCESS_DETACH) {
        //     if (bCrtInitResult1 == 0) {
        //         return FALSE;
        //     }
        // }

        int bSetModuleResult = Platform::GDIDrawingContext::setModuleHandle(hinstDLL);

        // if (fdwReason == DLL_PROCESS_ATTACH) {
        //     if (bSetModuleResult != 0) {
        //         return bSetModuleResult;
        //     }
        //     // Cleanup if setModuleHandle fails
        //     crtInitHandler(hinstDLL, DLL_PROCESS_DETACH); 
        // }

        if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH) {
            return bSetModuleResult;
        }

        // int bCrtInitResult2 = crtInitHandler(hinstDLL, fdwReason);
        // int bFinalResult = bSetModuleResult;
        // if (bCrtInitResult2 == 0) {
        //     bFinalResult = 0;
        // }

        // if (bFinalResult != 0) {
        //     if (g_pfnCustomInit != nullptr) {
        //         return (*g_pfnCustomInit)(hinstDLL, dwOriginalReason, lpvReserved);
        //     }
        //     return bFinalResult;
        // }

        return FALSE;
    }
}
}