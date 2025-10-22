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

# Install dependencies with explicit triplet.
packages=$(cat "dependencies.txt")
for p in $packages
do
    echo "Installing $p for x64-linux..."
    vcpkg/vcpkg install $p:x64-linux
done

echo "All vcpkg dependencies installed successfully!"
echo "Installed packages:"
vcpkg/vcpkg list