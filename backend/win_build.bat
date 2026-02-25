@echo off
setlocal enabledelayedexpansion

:: Argument loading by Garret Wilson: https://stackoverflow.com/a/50652990

set buildDirectory=build

set releaseMode=
set defaultReleaseMode=Debug
set enableTests=false

:args
set parameter=%~1
set argument=%~2

if "%parameter%" == "--mode"  (
    if not "%argument%" == "" (
        set releaseMode=%argument%
        shift
        shift
    ) else (
        set releaseMode=%defaultReleaseMode%
    )
) else if "%parameter%" == "-m" (
    shift
    if not "%argument%" == "" (
        set releaseMode=%argument%
        shift
        shift
    ) else (
        set releaseMode=%defaultReleaseMode%
    )
) else if "%parameter%" == "--test" (
    shift
    set enableTests=true
) else if "%parameter%" == "-t" (
    shift
    set enableTests=true
) else if "%parameter%" == "" (
    goto endargs
) else (
    echo Unrecognized option '%parameter%'.
    goto usage
)
goto args
:endargs

:: End argument loading

if "%releaseMode%" == "" (
    set releaseMode=%defaultReleaseMode%
) 

if not "%releaseMode%" == "Debug" (
    if not "%releaseMode%" == "Release" (
        echo Mode needs to be set to Debug or Release. Currently set to: "%releaseMode%"
        exit /b 1
    )
)

set cmakeArgs=

if "%enableTests%" == "true" (
    set cmakeArgs=-DENABLE_TESTS=ON -DENABLE_COVERAGE=ON
)

cmake -B "%buildDirectory%" -S . -D CMAKE_BUILD_TYPE="%releaseMode%" !cmakeArgs!

if errorlevel 1 (
    echo Failed to create cmake project. Make sure you have run ./windows_config_environment.bat
    exit /b
)

cd "%buildDirectory%"

if errorlevel 1 (
    echo Failed to navigate to build directory.
    exit /b
)

cmake --build . -j12 --config "%releaseMode%" -- /m:%NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo Failed to build project
    exit /b
)

cd ..

:: Copy .exe and libraries
mkdir "..\frontend\lib\"
echo Copy to ".\build\%releaseMode%\"
xcopy /y ".\build\%releaseMode%\" "..\frontend\lib\"

:: Copy all backend resource files
mkdir "..\frontend\res\"
xcopy /y /e ".\res\" "..\frontend\res\"

goto :eof

:usage
echo usage: win_build [-t ^| --test] [-m ^| --mode] (Debug ^| Release)
echo Options:
echo   -t ^| --test           Build tests.
echo   -m ^| --mode           Set the build can be: Debug ^| Release
:endusage
