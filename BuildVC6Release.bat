@echo off
call "%~dp0LocalPaths.bat"
call "%MSVC600_PATH%\VC98\Bin\vcvars32.bat"
cmake --build --preset vc6-local