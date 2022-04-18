#!/bin/sh

# Make sure the release directory is gone so that the 
# frontend does not automatically start the old executable.
if [ -d ../frontend/lib/ ]; then rm -rf ../frontend/lib/; fi
if [ -d ../frontend/res/ ]; then rm -rf ../frontend/res/; fi

# Run CMake
cmake -B ./build/Release -S . -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-osx
if [ $? -ne 0 ]; then exit; fi
cmake --build ./build/Release # --config Release
if [ $? -ne 0 ]; then exit; fi

# Copy executable over
mkdir ../frontend/lib/
cp -v ./build/Release/app ../frontend/lib/

# Copy resource files over.
mkdir ../frontend/res/
cp -rv ./res/* ../frontend/res/
