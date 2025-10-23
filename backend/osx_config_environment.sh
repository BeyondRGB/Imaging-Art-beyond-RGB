#!/bin/sh
minimum_macOS_version=10.15

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Check for vcpkg updates. Exit if fail.
cd vcpkg || exit
git pull --ff-only
./bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done