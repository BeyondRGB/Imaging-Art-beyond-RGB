@echo off
setlocal

if not defined VCPKG_ROOT set "VCPKG_ROOT=%CD%\vcpkg"

::: If vcpkg not set-up, clone and bootstrap
if not exist "%VCPKG_ROOT%\bootstrap-vcpkg.bat" (
    echo Setting up vcpkg...
    if not exist "%VCPKG_ROOT%" (
        git clone https://github.com/microsoft/vcpkg "%VCPKG_ROOT%"
        if errorlevel 1 (
            echo Failed to clone vcpkg
            exit /b 1
        )
    )
)

:: Update vcpkg
pushd "%VCPKG_ROOT%"
git pull --ff-only
call bootstrap-vcpkg.bat
if errorlevel 1 (
    echo Failed to bootstrap vcpkg
    popd
    exit /b 1
)
popd

::: Install dependencies
echo Installing dependencies...
for /F "tokens=*" %%A in (dependencies.txt) do (
    echo Installing %%A...
    "%VCPKG_ROOT%\vcpkg.exe" install %%A:x64-windows
)

echo Environment configured successfully!

endlocal

