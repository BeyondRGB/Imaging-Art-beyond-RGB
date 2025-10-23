@echo off

:: If vcpkg not set-up, clone and bootstrap
if not exist "vcpkg\" (
    echo Setting up vcpkg...
    git clone https://github.com/microsoft/vcpkg
    call vcpkg\bootstrap-vcpkg.bat
    if errorlevel 1 (
        echo Failed to bootstrap vcpkg
        exit /b 1
    )
)

:: Update vcpkg
cd vcpkg
git pull --ff-only
call bootstrap-vcpkg.bat
cd ..

:: Install dependencies
echo Installing dependencies...
for /F "tokens=*" %%A in (dependencies.txt) do (
    echo Installing %%A...
    vcpkg\vcpkg.exe install %%A:x64-windows
)

echo Environment configured successfully!

