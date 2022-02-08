#!/bin/sh

# Make sure the release directory is gone so that the 
# frontend does not automatically start the old executable.
if [ -d ../frontend/backend/ ]
then
	rm -rf ../frontend/backend/
fi

# Make sure the resource folder exists.
if [ -d ./build/Debug/resources/app/backend/res/ ]
then
	echo Found build/Debug
else
	echo Creating build/Debug
	mkdir -p ./build/Debug/resources/app/backend/res/
fi

# Copy resource files over.
cp -rv ./res/* ./build/Debug/resources/app/backend/res/

# Run CMake
cmake -B ./build/Debug -S . -D CMAKE_BUILD_TYPE=Debug -D VCPKG_TARGET_TRIPLET=x64-linux
cmake --build ./build/Debug # --config Debug
