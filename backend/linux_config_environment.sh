#!/bin/sh

# Ensure a healthy vcpkg checkout (handle partial caches)
if [ ! -d "vcpkg" ] || [ ! -d "vcpkg/.git" ] || [ ! -f "vcpkg/bootstrap-vcpkg.sh" ]; then
    rm -rf vcpkg
    git clone https://github.com/microsoft/vcpkg
    sh vcpkg/bootstrap-vcpkg.sh
else
    cd vcpkg || exit
    git pull --ff-only
    sh bootstrap-vcpkg.sh
    cd ..
fi

# Set default triplet for Linux
TRIPLET="x64-linux"

echo "Installing dependencies for Linux ($TRIPLET)..."

# Install dependencies with explicit triplet.
packages=$(cat "dependencies.txt")
for p in $packages
do
    echo "Installing $p for $TRIPLET..."
    if ! vcpkg/vcpkg install $p:$TRIPLET; then
        echo "Warning: Failed to install $p for $TRIPLET, trying without triplet..."
        vcpkg/vcpkg install $p
    fi
done

echo "All vcpkg dependencies installed successfully!"
echo "Installed packages:"
vcpkg/vcpkg list