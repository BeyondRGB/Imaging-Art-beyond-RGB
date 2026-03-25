#!/bin/sh

mode=Debug

# Build the backend
sh unix_build.sh -m ${mode}

if [ -d tmp ]; then rm -rf tmp; fi

mkdir tmp

echo "Copying executable to the frontend."
# Copy executable over
mkdir tmp/lib
cp -v "build/${mode}/beyond-rgb-backend" tmp/lib/

echo "Copying resources to the frontend."
# Copy resource files over.
mkdir tmp/res
cp -rv ./res/* tmp/res/

./tmp/lib/beyond-rgb-backend

#rm -rf tmp