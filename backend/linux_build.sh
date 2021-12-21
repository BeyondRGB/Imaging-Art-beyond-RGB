#!/bin/sh
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-linux
cmake --build build