cmake_minimum_required(VERSION 3.15)

project(DelayLama CXX)

# Generate compile_commands.json for clangd intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Set 32-bit compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2 /DWIN32")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /machine:x86")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /machine:x86")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /machine:x86")

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Matches original pe name and exports
add_library(DelayLama SHARED
    ${SOURCES}
    exports.def
    resources.rc
)

set_target_properties(DelayLama PROPERTIES
    PREFIX ""
    OUTPUT_NAME "DelayLama"
)
target_link_options(DelayLama PRIVATE "/IGNORE:4070")

# Windows specifics
target_link_libraries(DelayLama
    kernel32
)


# Add include directory for headers
target_include_directories(DelayLama PRIVATE src/)