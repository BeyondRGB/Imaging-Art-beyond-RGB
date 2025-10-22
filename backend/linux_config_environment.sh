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