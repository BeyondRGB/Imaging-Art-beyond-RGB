#!/bin/sh

# Make sure the release directory is gone so that the 
# frontend does not automatically start the old executable.
if [ -d ../frontend/lib/ ]
then
	rm -rf ../frontend/lib/
fi

# Make sure the resource folder exists.
if [ -d ./build/Release/res/ ]
then
	echo Found build/Release
else
	echo Creating build/Release
	mkdir -p ./build/Release/res/
fi

# Copy resource files over.
cp -rv ./res/* ./build/Release/res/

# Run CMake
cmake -B ./build/Release -S . -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-linux
cmake --build ./build/Release # --config Release