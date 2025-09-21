#!/bin/sh

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg
    sh vcpkg/bootstrap-vcpkg.sh
fi

# Check for vcpkg updates.
cd vcpkg
git pull --ff-only
sh bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done