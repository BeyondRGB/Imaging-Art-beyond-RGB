#!/bin/sh
macOS_vers_num=10.15

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg
    sh vcpkg/bootstrap-vcpkg.sh

    # Append minimum macOS version to vcpkg/triplets/x64-osx.cmake
    echo "set(VCPKG_C_FLAGS -mmacosx-version-min=$macOS_vers_num)" >> vcpkg/triplets/x64-osx.cmake
    echo "set(VCPKG_CXX_FLAGS -mmacosx-version-min=$macOS_vers_num)" >> vcpkg/triplets/x64-osx.cmake
fi

# Check for vcpkg updates.
cd vcpkg
git pull --ff-only
sh vcpkg/bootstrap-vcpkg.sh
cd ..

# Install dependencies.
export MACOSX_DEPLOYMENT_TARGET=$macOS_vers_num
export VCPKG_OSX_DEPLOYMENT_TARGET=$macOS_vers_num
export VCPKG_KEEP_ENV_VARS="VCPKG_OSX_DEPLOYMENT_TARGET;MACOSX_DEPLOYMENT_TARGET"
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done