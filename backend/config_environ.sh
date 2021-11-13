#!/bin/sh

# macOS:
#   brew install cmake
#   brew install pkg-config

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg
    sh vcpkg/bootstrap-vcpkg.sh
fi

# Check for vcpkg updates.
cd vcpkg
git pull --ff-only
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done