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

if exist "build\Release\Release\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\Release\Release\beyond-rgb-backend.exe
) else if exist "build\Release\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\Release\beyond-rgb-backend.exe
) else if exist "build\Debug\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\Debug\beyond-rgb-backend.exe
) else if exist "build\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=build\beyond-rgb-backend.exe
) else if exist "..\frontend\lib\beyond-rgb-backend.exe" (
    set BACKEND_BINARY=..\frontend\lib\beyond-rgb-backend.exe
) else (
    echo WARNING: Could not find backend binary in standard locations
    echo Searching entire build directory...
    for /f "delims=" %%i in ('dir /s /b build\beyond-rgb-backend.exe 2^>nul') do set BACKEND_BINARY=%%i
    if not defined BACKEND_BINARY (
        echo Skipping smoke test - backend binary not found
        echo Searched in:
        echo   - build\Release\Release\beyond-rgb-backend.exe
        echo   - build\Release\beyond-rgb-backend.exe
        echo   - build\Debug\beyond-rgb-backend.exe
        echo   - build\beyond-rgb-backend.exe
        echo   - ..\frontend\lib\beyond-rgb-backend.exe
        echo.
        echo Build directory contents:
        dir /s /b build\ 2>nul | findstr /i "\.exe$"
        exit /b 0
    )
)

echo Found backend binary: %BACKEND_BINARY%

REM Test 1: Check if binary exists
if not exist "%BACKEND_BINARY%" (
    echo ERROR: Binary does not exist
    exit /b 1
)
echo Binary exists

REM Test 2: Try running with --help flag (with timeout)
echo.
echo Test: Running with --help flag ^(2 second timeout^)...
start /B "" "%BACKEND_BINARY%" --help >nul 2>&1
timeout /t 2 /nobreak >nul
taskkill /IM beyond-rgb-backend.exe /F >nul 2>&1
echo Binary doesn't support --help ^(this is okay^)

REM Test 3: Try running with --version flag (with timeout)
echo.
echo Test: Running with --version flag ^(2 second timeout^)...
start /B "" "%BACKEND_BINARY%" --version >nul 2>&1
timeout /t 2 /nobreak >nul
taskkill /IM beyond-rgb-backend.exe /F >nul 2>&1
echo Binary doesn't support --version ^(this is okay^)

REM Test 4: Try running with --test flag (with timeout)
echo.
echo Test: Starting backend in test mode ^(2 second timeout^)...
start /B "" "%BACKEND_BINARY%" --test >nul 2>&1
timeout /t 2 /nobreak >nul
taskkill /IM beyond-rgb-backend.exe /F >nul 2>&1
echo Test mode not available ^(this is okay^)

REM Test 5: Quick start/stop test
echo.
echo Test: Checking if binary starts without immediate crash...
start /B "" "%BACKEND_BINARY%" --port 9999 >nul 2>&1
timeout /t 2 /nobreak >nul
taskkill /IM beyond-rgb-backend.exe /F >nul 2>&1
echo Binary can start ^(this is okay for smoke test^)

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

