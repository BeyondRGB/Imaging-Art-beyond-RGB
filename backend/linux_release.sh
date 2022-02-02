#!/bin/sh
if [ -d ./build/Release/resouces/app/backend/res/ ]
then
	echo Found ./build/Release/resouces/app/backend/res/
else
	echo The stuff
	mkdir -p ./build/Release/resouces/app/backend/res/
fi
# Copy all backend resource files relative to where they will live in deploed app
cp -r ./res/* ./build/Release/resouces/app/backend/res/

# Build
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build ./build/ --config Release
