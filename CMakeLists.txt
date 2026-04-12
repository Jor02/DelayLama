cmake_minimum_required(VERSION 3.15)

project(DelayLama CXX)

# Generate compile_commands.json for clangd intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

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