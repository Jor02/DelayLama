@echo off
call "%~dp0LocalPaths.bat"

if not exist "%MSVC600_PATH%\VC98\Bin\VCVARS32.BAT" (
    echo VCVARS32.BAT not found!
    exit /b 1
)

call "%MSVC600_PATH%\VC98\Bin\VCVARS32.BAT"
cmake --build --preset vc6-debug