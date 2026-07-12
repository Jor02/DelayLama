@echo off
call "%~dp0LocalPaths.bat"

if not exist "%MSVC600_PATH%\VC98\Bin\VCVARS32.BAT" (
    echo VCVARS32.BAT not found in "%MSVC600_PATH%\VC98\Bin"
    exit /b 1
)

call "%MSVC600_PATH%\VC98\Bin\VCVARS32.BAT"

set CMAKE_ARGS=

if "%1"=="--console" (
    set CMAKE_ARGS=-DDELAYLAMA_DEBUG_CONSOLE=ON
)

cmake --preset vc6 %CMAKE_ARGS%
cmake --preset vc6-debug %CMAKE_ARGS%