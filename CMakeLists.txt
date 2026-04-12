cmake_minimum_required(VERSION 3.15)

project(DelayLama CXX)

# Generate compile_commands.json for clangd intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Detemine target architecture
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    # Map Visual Studio platform names to desired folder names
    if(CMAKE_VS_PLATFORM_NAME STREQUAL "Win32")
        set(OUTPUT_ARCH "x32")
    elseif(CMAKE_VS_PLATFORM_NAME STREQUAL "x64")
        set(OUTPUT_ARCH "x64")
    else()
        set(OUTPUT_ARCH "${CMAKE_VS_PLATFORM_NAME}")
    endif()
else()
    # For other generators (NMake, Makefiles) we expect ARCH to be set in the toolchain file
    if(NOT DEFINED ARCH)
        message(FATAL_ERROR "ARCH not defined for non-VS generator")
    endif()
    set(OUTPUT_ARCH "${ARCH}")
endif()

if(NOT CMAKE_TOOLCHAIN_OUTPUT_NAME)
    set(CMAKE_TOOLCHAIN_OUTPUT_NAME "default")
endif()
set(OUTPUT_BASE "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_TOOLCHAIN_OUTPUT_NAME}")

# Set output directories
if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (Visual Studio)
    foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${OUTPUT_BASE}/${OUTPUT_ARCH}/${config}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${OUTPUT_BASE}/${OUTPUT_ARCH}/${config}")
    endforeach()
else() # Single-config generator (NMake, Makefiles)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE}/${OUTPUT_ARCH}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_BASE}/${OUTPUT_ARCH}/${CMAKE_BUILD_TYPE}")
endif()

# Collect sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Shared library (DLL) matching original name and exports
add_library(DelayLama SHARED
    ${SOURCES}
    exports.def
    resources.rc
)

set_target_properties(DelayLama PROPERTIES
    PREFIX ""
    OUTPUT_NAME "DelayLama"
    SUFFIX ".DLL"
)

# Add include directory for headers
target_include_directories(DelayLama PRIVATE src/)

# Windows libraries
target_link_libraries(DelayLama
    kernel32
    user32
    gdi32
)

# Linker warning suppression
target_link_options(DelayLama PRIVATE "/IGNORE:4070")