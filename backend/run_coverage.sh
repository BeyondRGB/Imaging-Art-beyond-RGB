#!/bin/sh
set -eu

mode="${1:-Debug}"

if [ "$mode" != "Debug" ] && [ "$mode" != "Release" ]; then
    echo "Mode needs to be Debug or Release. Currently set to: $mode"
    exit 1
fi

./unix_build.sh --test --mode "$mode"
cmake --build "build/$mode" --target coverage

echo "Coverage report generated at build/$mode/coverage/html/index.html"
