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
set "VCPKG_EXE=%VCPKG_ROOT%\vcpkg.exe"
if not exist "%VCPKG_EXE%" (
    for /f "delims=" %%I in ('where vcpkg.exe 2^>nul') do (
        set "VCPKG_EXE=%%I"
        goto :found_vcpkg_exe
    )
    if exist "%VCPKG_ROOT%\vcpkg" (
        set "VCPKG_EXE=%VCPKG_ROOT%\vcpkg"
        goto :found_vcpkg_exe
    )
    echo Failed to locate vcpkg executable after bootstrap.
    popd
    exit /b 1
)
:found_vcpkg_exe
popd

::: Install dependencies
echo Installing dependencies...
for /F "tokens=*" %%A in (dependencies.txt) do (
    echo Installing %%A...
    "%VCPKG_EXE%" install %%A:x64-windows
)

echo Environment configured successfully!

endlocal

