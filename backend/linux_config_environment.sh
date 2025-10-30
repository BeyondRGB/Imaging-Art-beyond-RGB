#!/bin/sh

# If vcpkg not set-up, or bootstrap script missing, clone and bootstrap.
if ! [ -f "vcpkg/bootstrap-vcpkg.sh" ]; then
    rm -rf vcpkg
    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Check for vcpkg updates.
cd vcpkg
git pull --ff-only || true
./bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    ./vcpkg/vcpkg install $p:x64-linux
done