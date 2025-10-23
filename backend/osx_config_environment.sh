#!/bin/sh
minimum_macOS_version=10.15

# If vcpkg not set-up, or bootstrap script missing, clone and bootstrap.
if ! [ -f "vcpkg/bootstrap-vcpkg.sh" ]; then
    rm -rf vcpkg
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Check for vcpkg updates. Exit if fail.
cd vcpkg || exit
git pull --ff-only || true
./bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    ./vcpkg/vcpkg install $p
done