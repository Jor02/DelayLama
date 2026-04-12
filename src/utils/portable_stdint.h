#pragma once

/* Check if we are on a modern compiler that already has stdint.h */
#ifdef _MSC_VER
    #if _MSC_VER >= 1600 // Visual Studio 2010+
        #include <cstdint>
    #else
        // We are in MSVC 6.0 (or older), define manually.
        #define override
        #define nullptr NULL 

        typedef signed char        int8_t;
        typedef unsigned char      uint8_t;
        typedef short              int16_t;
        typedef unsigned short     uint16_t;
        
        typedef long               int32_t;
        typedef unsigned long      uint32_t;
        
        typedef __int64            int64_t;
        typedef unsigned __int64   uint64_t;

        #ifdef _WIN64
            typedef unsigned __int64 size_t;
            typedef __int64          ptrdiff_t;
            typedef __int64          intptr_t;
        #else
            typedef unsigned int     size_t;
            typedef int              ptrdiff_t;
            typedef int              intptr_t;
        #endif
    #endif
#else
    /* GCC / Clang / Modern Unix */
    #if __STDC_VERSION__ >= 199901L
        #include <cstdint>
    #else
        #error "Compiler too old and not MSVC6. Cannot guarantee int sizes."
    #endif
#endif