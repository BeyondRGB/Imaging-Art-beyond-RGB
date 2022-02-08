#!/bin/sh

# Make sure the release directory is gone so that the 
# frontend does not automatically start the old executable.
if [ -d ../frontend/backend/ ]
then
	rm -rf ../frontend/backend/
fi

# Make sure the resource folder exists.
if [ -d ./build/Release/resources/app/backend/res/ ]
then
	echo Found build/Release
else
	echo Creating build/Release
	mkdir -p ./build/Release/resources/app/backend/res/
fi

# Copy resource files over.
cp -rv ./res/* ./build/Release/resources/app/backend/res/

# Run CMake
cmake -B ./build/Release -S . -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-linux
cmake --build ./build/Release # --config Release