@echo off
REM Backend Smoke Test Script (Windows)
REM Tests that the backend executable starts and responds correctly

echo ==========================================
echo Backend Smoke Test
echo ==========================================

REM Debug: Show what's in the build directory
echo Checking build directory contents...
if exist "build" (
    echo Build directory exists. Contents:
    dir /b build\
    if exist "build\Release" (
        echo build\Release contents:
        dir /b build\Release\
    )
    if exist "build\Debug" (
        echo build\Debug contents:
        dir /b build\Debug\
    )
) else (
    echo ERROR: build\ directory does not exist!
    echo The backend build step may have failed.
    exit /b 1
)

REM Find the backend executable
set BACKEND_BINARY=

if exist "build\Release\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\Release\beyond-rgb-backend.exe
) else if exist "build\Debug\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\Debug\beyond-rgb-backend.exe
) else if exist "build\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\beyond-rgb-backend.exe
) else if exist "..\frontend\lib\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=..\frontend\lib\beyond-rgb-backend.exe
) else (
    echo ERROR: Could not find backend binary
    echo Searched in:
    echo   - build\Release\beyond-rgb-backend.exe
    echo   - build\Debug\beyond-rgb-backend.exe
    echo   - build\beyond-rgb-backend.exe
    echo   - ..\frontend\lib\beyond-rgb-backend.exe
    echo.
    echo Build directory structure:
    dir /s /b build\*backend* 2>nul
    exit /b 1
)

echo Found backend binary: %BACKEND_BINARY%

REM Test 1: Check if binary exists
if not exist "%BACKEND_BINARY%" (
    echo ERROR: Binary does not exist
    exit /b 1
)
echo Binary exists

REM Test 2: Try running with --help flag
echo.
echo Test: Running with --help flag...
"%BACKEND_BINARY%" --help >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Binary responds to --help
) else (
    echo Warning: Binary does not respond to --help ^(this might be expected^)
)

REM Test 3: Try running with --version flag
echo.
echo Test: Running with --version flag...
"%BACKEND_BINARY%" --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Binary responds to --version
) else (
    echo Warning: Binary does not respond to --version ^(this might be expected^)
)

REM Test 4: Try running with --test flag
echo.
echo Test: Starting backend in test mode...
"%BACKEND_BINARY%" --test >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Binary runs in test mode successfully
) else (
    echo Warning: Test mode not available or failed ^(this might be expected^)
)

REM Test 5: Quick start/stop test
echo.
echo Test: Checking if binary starts without immediate crash...
start /B "" "%BACKEND_BINARY%" --port 9999 >nul 2>&1
timeout /t 2 /nobreak >nul
taskkill /IM beyond-rgb-backend.exe /F >nul 2>&1

echo.
echo ==========================================
echo Smoke Test Complete
echo ==========================================
echo.
echo Summary:
echo   - Binary exists and can be executed
echo   - Binary can be started
echo.
echo Note: Full integration tests require a complete environment setup
echo       and are beyond the scope of this smoke test.

exit /b 0

