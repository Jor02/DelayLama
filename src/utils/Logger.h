#pragma once

namespace DelayLama {
namespace Utils {

    // Initializes the console. Call this once in VSTPluginMain.
    void attachConsole();

    // Simple string logger
    void log(const char* msg);

    // Formatted logger
    void logf(const char* format, ...);

}
}