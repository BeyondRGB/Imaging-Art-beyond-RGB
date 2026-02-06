@echo off
setlocal enabledelayedexpansion

if not exist vcpkg (
    git clone https://github.com/microsoft/vcpkg
)

:: Navigate to vcpkg directory so we can update. If not directory fail.
cd vcpkg

if errorlevel 1 (
    echo Failed to find vcpkg directory.
    exit /b
)

:: Check for vcpkg updates
git pull --ff-only
cmd /c .\vcpkg\bootstrap-vcpkg.bat
cd ..

:: Install dependencies
for /F "tokens=*" %%A in (dependencies.txt) do .\vcpkg\vcpkg.exe install %%A:x64-windows
