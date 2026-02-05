#!/bin/sh
build_directory="build"

usage() {
    echo "usage $0 [-t | --test] [-m | --mode] (Debug | Release)"
    echo "Options:"
    echo "  -t | --test           Build tests."
    echo "  -m | --mode           Set the build can be: Debug | Release"
}

# Transform long options to short ones: https://stackoverflow.com/a/30026641
for arg in "$@"; do
  shift
  case "$arg" in
	'--test') set -- "$@" '-t'   ;;
#	'--triplet') set -- "$@" '-T'   ;;
	'--mode') set -- "$@" '-m'   ;;
	*) set -- "$@" "$arg" ;;
  esac
done

while getopts "tTm" opt; do
	case "$opt" in
		't') build_tests=true ;;
#		'T') add_triplet=true ;;
		'm') mode=$OPTARG ;;
		*) printf "Invalid argument: %s" "$opt" ;;
	esac
done

if [ -z ${mode+x} ]; then
	echo "Mode needs to be set to Debug or Release"
	usage
	exit 1
elif ! [ "${mode}" = "Debug" ] && ! [ "${mode}" = "Release" ]; then
    echo "Mode needs to be set to Debug or Release"
    usage
    exit 1
fi

# Run CMake
set_cmake_args=false

#if  [ "$add_triplet" = true ]; then
#    set -- "-DVCPKG_TARGET_TRIPLET=$(uname -m)"
#    set_cmake_args=true
#fi

if [ "$build_tests" = true ]; then
  set -- "-DENABLE_TESTS=ON" "-DENABLE_COVERAGE=ON"
  set_cmake_args=true
fi

# Clear arguments so we don't just pass the program arguments to cmake.
if [ "$set_cmake_args" = false ]; then
    set --
fi

echo "$@"

cmake -B "build/${mode}" -S . -D CMAKE_BUILD_TYPE="$mode" "$@"

if [ $? -ne 0 ]; then
	echo "Failed to create cmake project. Make sure you have run ./unix_config_environment.sh."
	exit
fi

exit

cd "$build_directory/${mode}" || exit

if [ "$(uname)" = "Darwin" ]; then
	cmake --build . -j"$(sysctl -n hw.logicalcpu)"
else
	cmake --build . -j"$(nproc)"
fi

if [ $? -ne 0 ]; then
	echo "Failed to build project."
	exit
fi

# Go back to root of the script
cd ../.. || exit

if [ -d ../frontend/ ]; then
	# Make sure the release directory is gone so that the
	# frontend does not automatically start the old executable.
	echo "Removing old versions of the backend from the frontend."
	if [ -d ../frontend/lib/ ]; then rm -rf ../frontend/lib/; fi
	if [ -d ../frontend/res/ ]; then rm -rf ../frontend/res/; fi

	echo "Copying executable to the frontend."
	# Copy executable over
	mkdir ../frontend/lib/
	cp -v "build/${mode}/beyond-rgb-backend" ../frontend/lib/

	echo "Copying resources to the frontend."
	# Copy resource files over.
	mkdir ../frontend/res/
	cp -rv ./res/* ../frontend/res/
else
	echo "! Could not find the frontend files to copy the backend into..."
fi
