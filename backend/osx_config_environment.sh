#!/bin/sh
minimum_macOS_version=10.15

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

# Determine architecture
if [ "$(uname -m)" = "arm64" ]; then
    TRIPLET="arm64-osx"
else
    TRIPLET="x64-osx"
fi

echo "Installing dependencies for $TRIPLET..."

# Install dependencies with explicit triplet.
packages=$(cat "dependencies.txt")
for p in $packages
do
    echo "Installing $p for $TRIPLET..."
    vcpkg/vcpkg install $p:$TRIPLET
done

echo "All vcpkg dependencies installed successfully!"
echo "Installed packages:"
vcpkg/vcpkg list