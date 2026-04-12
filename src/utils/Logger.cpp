#include <windows.h>
#include <cstdio>
#include <cstdarg>
#include "Logger.h"

// This stays static so it's only visible inside Logger.cpp
static bool g_consoleAttached = false;

namespace Utils {

    void attachConsole()
    {
        if (g_consoleAttached) return;

        if (AllocConsole())
        {
#if _MSC_VER >= 1400  // VS2005 and later have freopen_s
            FILE* fDummy;
            freopen_s(&fDummy, "CONOUT$", "w", stdout);
            freopen_s(&fDummy, "CONOUT$", "w", stderr);
#else
            // VC6 uses plain freopen (returns FILE*)
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
#endif
            setvbuf(stdout, NULL, _IONBF, 0);
            setvbuf(stderr, NULL, _IONBF, 0);

            g_consoleAttached = true;
            printf("Console attached from Delay Lama reimplementation\n");
        }
        else
        {
            log("AllocConsole failed\n");
        }
    }

    void log(const char* msg)
    {
        if (!g_consoleAttached) {
            OutputDebugStringA(msg);
        } else {
            printf("%s", msg);
            fflush(stdout);
        }
    }

    void logf(const char* format, ...)
    {
        char buffer[1024];
        va_list args;

        va_start(args, format);
#if _MSC_VER >= 1400
        vsnprintf(buffer, sizeof(buffer), format, args);
#else
        _vsnprintf(buffer, sizeof(buffer) - 1, format, args);
        buffer[sizeof(buffer) - 1] = '\0'; // ensure null termination
#endif
        va_end(args);

        if (!g_consoleAttached) {
            OutputDebugStringA(buffer);
        } else {
            printf("%s", buffer);
            fflush(stdout);
        }
    }

}