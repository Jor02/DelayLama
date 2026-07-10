@echo off
setlocal enabledelayedexpansion

set "PROJECT_ROOT=%~dp0"
cd /d "%PROJECT_ROOT%"

REM --------------------------------------------------
REM Build the VC6 project
REM --------------------------------------------------
echo Building project with VC6...
call BuildVC6Release.bat
if errorlevel 1 (
    echo ERROR: Build failed.
    exit /b 1
)

REM --------------------------------------------------
REM Activate Python virtual environment
REM --------------------------------------------------
echo Activating virtual environment...
if not exist ".venv\Scripts\activate.bat" (
    echo ERROR: Virtual environment not found at .venv\Scripts\activate.bat
    exit /b 1
)
call .venv\Scripts\activate.bat

REM --------------------------------------------------
REM Prepare reccmp analysis
REM --------------------------------------------------
echo Preparing reccmp analysis...

set "BUILD_OUT_DIR=build\bin\vc6\x32\RelWithDebInfo"
pushd "%BUILD_OUT_DIR%"

if not exist "reccmp-build.yml" (
    echo reccmp-build.yml missing - generating with 'reccmp-project detect --what recompiled'
    reccmp-project detect --what recompiled
    if errorlevel 1 (
        popd
        call .venv\Scripts\deactivate.bat
        exit /b 1
    )
)

REM --------------------------------------------------
REM Run reccmp comparison
REM --------------------------------------------------
echo Running reccmp-reccmp...
reccmp-reccmp --target DELAYLAMA --html ../../../../../progress.html --silent
set "RECCMP_EXIT_CODE=%errorlevel%"

popd

REM --------------------------------------------------
REM Deactivate virtual environment
REM --------------------------------------------------
call .venv\Scripts\deactivate.bat

REM --------------------------------------------------
REM Finish
REM --------------------------------------------------
if %RECCMP_EXIT_CODE% equ 0 (
    echo Success! Comparison finished.
) else (
    echo reccmp-reccmp exited with code %RECCMP_EXIT_CODE%.
)
exit /b %RECCMP_EXIT_CODE%