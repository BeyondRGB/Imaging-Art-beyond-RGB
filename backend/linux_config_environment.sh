#!/bin/sh

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Check for vcpkg updates.
cd vcpkg
git pull --ff-only
./bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done