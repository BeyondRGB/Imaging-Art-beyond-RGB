#!/bin/sh
if [ -d ./build/resources/app/backend/res/ ]
then
	echo Found ./build/resources/app/backend/res/
else
	mkdir -p ./build/resources/app/backend/res/
fi
# Copy all backend resource files relative to where they will live in deployed app
cp -r ./res/* ./build/resources/app/backend/res/

# Build
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-osx
cmake --build ./build/ --config Release
