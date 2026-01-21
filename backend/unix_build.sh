#!/bin/sh
build_directory="build"
mode=$1

if ! [ "${mode}" = "Debug" ] && ! [ "${mode}" = "Release" ]; then
    echo "Mode not set to Debug or Release."
    echo "usage: unix_build.sh [Debug | Release]"
    exit
fi

# Run CMake
cmake -B "build/${mode}" -S . -D CMAKE_BUILD_TYPE=$mode

if [ $? -ne 0 ]; then
    echo "Failed to create cmake project. Make sure you have run ./unix_config_environment.sh."
    exit
fi

cd "$build_directory/${mode}" || exit

if [ $(uname) = "Darwin" ]; then
  cmake --build . -j$(sysctl -n hw.logicalcpu)
else
    cmake --build . -j$(nproc)
fi

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
