#!/bin/sh
build_directory="build"
mode=$1
enable_tests=${2:-OFF}
enable_coverage=${3:-OFF}

if ! [ "${mode}" = "Debug" ] && ! [ "${mode}" = "Release" ]; then
    echo "Mode not set to Debug or Release."
    echo "usage: linux_build.sh [Debug | Release] [ENABLE_TESTS=ON/OFF] [ENABLE_COVERAGE=ON/OFF]"
    exit
fi

# Run CMake
cmake_args="-B build/${mode} -S . -D CMAKE_BUILD_TYPE=$mode"
if [ "${enable_tests}" = "ON" ]; then
    cmake_args="$cmake_args -D ENABLE_TESTS=ON"
fi
if [ "${enable_coverage}" = "ON" ]; then
    cmake_args="$cmake_args -D ENABLE_COVERAGE=ON"
fi

cmake $cmake_args

if [ $? -ne 0 ]; then
    echo "Failed to create cmake project. Make sure you have run ./linux_config_environment.sh."
    exit
fi

cd "$build_directory/${mode}" || exit

cmake --build .

if [ $? -ne 0 ]; then
    echo "Failed to build project."
    exit
fi

# Go back to root of the script
cd ../.. || exit

if [ -d ../frontend/ ]; then
    # Make sure the release directory is gone so that the
    # frontend does not automatically start the old executable.
    echo "Removing old versions of the backend from the frontend."
    if [ -d ../frontend/lib/ ]; then rm -rf ../frontend/lib/; fi
    if [ -d ../frontend/res/ ]; then rm -rf ../frontend/res/; fi

    echo "Copying executable to the frontend."
    # Copy executable over
    mkdir ../frontend/lib/
    cp -v "build/${mode}/beyond-rgb-backend" ../frontend/lib/

    echo "Copying resources to the frontend."
    # Copy resource files over.
    mkdir ../frontend/res/
    cp -rv ./res/* ../frontend/res/
else
    echo "! Could not find the frontend files to copy the backend into..."
fi

