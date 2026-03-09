@echo off
REM Test runner script for Windows
REM Runs backend Catch2 tests, frontend Cypress tests, and collects code coverage

setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
set BACKEND_DIR=%SCRIPT_DIR%backend
set FRONTEND_DIR=%SCRIPT_DIR%frontend
set COVERAGE_DIR=%SCRIPT_DIR%coverage

echo === Running All Tests ===
echo.

REM Step 1: Build backend with tests enabled
echo [1/6] Building backend with tests enabled...
cd /d "%BACKEND_DIR%"

REM Check if vcpkg is configured
if not exist "vcpkg\installed" (
    echo Warning: vcpkg not configured. Running config script...
    call win_config_environment.bat
)

REM Build with tests and coverage flags
set BUILD_MODE=Debug
set BUILD_DIR=build\%BUILD_MODE%

echo Running CMake with tests enabled...
cmake -B "%BUILD_DIR%" -S . ^
    -D CMAKE_BUILD_TYPE=%BUILD_MODE% ^
    -D ENABLE_TESTS=ON ^
    -D CMAKE_CXX_FLAGS="%CMAKE_CXX_FLAGS% --coverage -fprofile-arcs -ftest-coverage" ^
    -D CMAKE_EXE_LINKER_FLAGS="%CMAKE_EXE_LINKER_FLAGS% --coverage" ^
    -D VCPKG_TARGET_TRIPLET=x64-windows

if errorlevel 1 (
    echo Failed to configure CMake
    exit /b 1
)

echo Building backend...
cmake --build "%BUILD_DIR%" --config %BUILD_MODE%

if errorlevel 1 (
    echo Failed to build backend
    exit /b 1
)

REM Step 2: Run backend tests
echo.
echo [2/6] Running backend Catch2 tests...
cd /d "%BUILD_DIR%\%BUILD_MODE%"

if not exist "unit_tests.exe" (
    echo Error: unit_tests.exe not found
    exit /b 1
)

unit_tests.exe
if errorlevel 1 (
    echo Backend tests failed
    exit /b 1
)

echo Backend tests passed!

REM Step 3: Generate backend coverage report
echo.
echo [3/6] Generating backend coverage report...
if not exist "%COVERAGE_DIR%\backend" mkdir "%COVERAGE_DIR%\backend"

REM Note: Coverage tools (lcov/gcov) may need to be installed separately on Windows
echo Backend coverage data generated in %BUILD_DIR%

REM Step 4: Install frontend dependencies
echo.
echo [4/6] Setting up frontend and running frontend tests...
cd /d "%FRONTEND_DIR%"

REM Install dependencies if needed
if not exist "node_modules" (
    echo Installing frontend dependencies...
    call npm install
)

REM Step 5: Run Cypress tests
echo [5/6] Running frontend unit and Cypress tests...
call npm run test:coverage
if errorlevel 1 (
    echo Frontend tests failed
    exit /b 1
)

echo Frontend tests passed!

REM Step 6: Generate frontend coverage report (if coverage plugin is configured)
if exist "%FRONTEND_DIR%\coverage" (
    echo.
    echo [6/6] Frontend coverage report available at %FRONTEND_DIR%\coverage
    if not exist "%COVERAGE_DIR%\frontend" mkdir "%COVERAGE_DIR%\frontend"
    xcopy /E /I /Y "%FRONTEND_DIR%\coverage\*" "%COVERAGE_DIR%\frontend\" > nul 2>&1
)

REM Summary
echo.
echo === All Tests Passed! ===
echo Backend coverage: %COVERAGE_DIR%\backend
echo Frontend coverage: %COVERAGE_DIR%\frontend (if available)

exit /b 0
