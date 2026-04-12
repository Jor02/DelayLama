set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_TOOLCHAIN_OUTPUT_NAME vc6)

# Set Architecture
set(ARCH "x32" CACHE STRING "Architecture for output paths")

# Set the compilers
set(MSVC6_ROOT "C:/Users/Jor02/AppData/Local/Programs/MSVC600")
set(CMAKE_C_COMPILER "${MSVC6_ROOT}/VC98/Bin/cl.exe")
set(CMAKE_CXX_COMPILER "${MSVC6_ROOT}/VC98/Bin/cl.exe")
set(CMAKE_LINKER "${MSVC6_ROOT}/VC98/Bin/link.exe")

# Compiler flags
set(CMAKE_CXX_FLAGS "/W3 /GX /D \"WIN32\" /D \"_WINDOWS\"" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "/Gm /Zi /Od /D \"_DEBUG\"" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /D \"NDEBUG\"" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO 
    "/Zi /O2 /Ob2 /Oy- /GX /D \"NDEBUG\" /D \"WIN32\" /D \"_WINDOWS\" /MT"
    CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "/Os /D \"NDEBUG\"" CACHE STRING "" FORCE)

# DLL linker flags
set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
    "/incremental:no /debug /nodefaultlib:\"libcpmt\""
    CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no" CACHE STRING "" FORCE)

# Static linker flags (used for static libs like .lib)
set(CMAKE_STATIC_LINKER_FLAGS "/machine:I386" CACHE STRING "" FORCE)

# Use static CRT
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)

# Serial build pool for VC6
set(CMAKE_JOB_POOL_COMPILE "msvc_vc6_pool" CACHE STRING "" FORCE)
set(CMAKE_JOB_POOL_LINK "msvc_vc6_pool" CACHE STRING "" FORCE)
set_property(GLOBAL PROPERTY JOB_POOLS "msvc_vc6_pool=1")