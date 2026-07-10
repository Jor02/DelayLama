@echo off
call "%~dp0LocalPaths.bat"
call "%MSVC600_PATH%\VC98\Bin\vcvars32.bat"

set CMAKE_ARGS=

if "%1"=="--console" (
    set CMAKE_ARGS=-DDELAYLAMA_DEBUG_CONSOLE=ON
)

cmake --preset vc6 %CMAKE_ARGS%
cmake --preset vc6-debug %CMAKE_ARGS%