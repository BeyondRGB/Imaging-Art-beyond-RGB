#!/bin/sh
if [ $(uname) = "Darwin" ]; then
  minimum_macOS_version=10.15
fi

# If vcpkg not set-up, clone and bootstrap.
if ! [ -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
    sh vcpkg/bootstrap-vcpkg.sh
fi

# Check for vcpkg updates. Exit if fail.
cd vcpkg || exit
git pull --ff-only
sh bootstrap-vcpkg.sh
cd ..

# Install dependencies.
packages=$(cat "dependencies.txt")
for p in $packages
do
    vcpkg/vcpkg install $p
done