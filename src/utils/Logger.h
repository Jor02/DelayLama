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

    // When logging is disabled, make these no-ops so they compile away entirely.
    // VC6 cannot inline variadic functions, so logf uses fixed-arity overloads.
    inline void attachConsole() {}
    inline void log(const char*) {}
    inline void logf(const char*) {}
    template <class A> inline void logf(const char*, A) {}
    template <class A, class B> inline void logf(const char*, A, B) {}
    template <class A, class B, class C> inline void logf(const char*, A, B, C) {}
    template <class A, class B, class C, class D> inline void logf(const char*, A, B, C, D) {}

#endif

}
