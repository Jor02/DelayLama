#pragma once

namespace Utils {

#ifdef DELAYLAMA_ENABLE_LOGGING

    // Initializes the console. Call this once in VSTPluginMain.
    void attachConsole();

    // Simple string logger
    void log(const char* msg);

    // Formatted logger
    void logf(const char* format, ...);

#else

    // When logging is disabled, make these no-ops so they compile away entirely
    inline void attachConsole() {}
    inline void log(const char*) {}
    inline void logf(const char*, ...) {}

#endif

}
