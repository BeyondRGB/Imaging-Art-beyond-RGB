#!/bin/sh
#brew install cmake
#brew install pkg-config
macOS_vers_num=10.15

# Make sure the release directory is gone so that the 
# frontend does not automatically start the old executable.
if [ -d ../frontend/lib/ ]; then rm -rf ../frontend/lib/; fi
if [ -d ../frontend/res/ ]; then rm -rf ../frontend/res/; fi

# Run CMake
export MACOSX_DEPLOYMENT_TARGET=$macOS_vers_num
export VCPKG_OSX_DEPLOYMENT_TARGET=$macOS_vers_num
export VCPKG_KEEP_ENV_VARS="VCPKG_OSX_DEPLOYMENT_TARGET;MACOSX_DEPLOYMENT_TARGET"
cmake -B ./build/Debug -S . -D CMAKE_OSX_DEPLOYMENT_TARGET=$macOS_vers_num -D CMAKE_BUILD_TYPE=Debug -D VCPKG_TARGET_TRIPLET=x64-osx
if [ $? -ne 0 ]; then exit; fi
cmake --build ./build/Debug # --config Debug
if [ $? -ne 0 ]; then exit; fi

# Copy executable over
mkdir ../frontend/lib/
cp -v ./build/Debug/beyond-rgb-backend ../frontend/lib/

# Copy resource files over.
mkdir ../frontend/res/
cp -rv ./res/* ../frontend/res/
