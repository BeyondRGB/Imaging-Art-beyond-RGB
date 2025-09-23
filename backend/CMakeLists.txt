cmake_minimum_required(VERSION 3.16)

# Sets the CMAKE Toolchain to the vcpkg installed using ./arch_config_environment
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# VCPKG on macOS requires system architectures to be set, so set those here.
# Requires a check for the right architecture (x_86 or arm64) as well as making sure
# it is not already defined to allow the user to override it using the command line.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND NOT DEFINED CMAKE_OSX_ARCHITECTURES )
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "Cache the architectures so vcpkg knows what macOS architectures to build for" FORCE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
        set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE INTERNAL "Cache the architectures so vcpkg knows what macOS architectures to build for" FORCE)
    else()
        message("Target architecture is unknown or different: ${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

# Trace VCPKG package finding (Use for debugging VCPKG errors)
set(VCPKG_TRACE_FIND_PACKAGE ON)

project(BeyondRGB DESCRIPTION "Imaging Art Beyond RGB" VERSION 2.2.0)

configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.h.in
    ${CMAKE_SOURCE_DIR}/src/version.h
    @ONLY)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

#------------------------------------------
#Add VCPKG provided find_package Here
#------------------------------------------
find_package(asio CONFIG REQUIRED)
find_package(TIFF REQUIRED)
find_package(jsoncons CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/opencv4")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/png")
find_package(PNG REQUIRED)

# LCMS and LibRAW
find_package(lcms REQUIRED)
find_package(LibRaw REQUIRED)

# Pthreads & OpenMP, Only needed on Linux & Windows sinc macOS includes these in the compiler.
if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx" AND NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-osx")
    find_package(Threads REQUIRED)
    find_package(OpenMP REQUIRED)
endif()

# Include websocketpp AFTER vcpkg is configured
set(BUILD_TESTS OFF CACHE BOOL "Disable tests in websocketpp" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples in websocketpp" FORCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/submodules/websocketpp)

# Find all Header and Source files
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Create .exe
add_executable(beyond-rgb-backend ${SOURCES})

# Set all source include files (ie. *.h)
target_include_directories(beyond-rgb-backend PUBLIC "src/")

# adds websocketpp as a target lib for compilation

target_include_directories(beyond-rgb-backend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/websocketpp
)
# adds extra modes for compilation, since we dont use boost and asio instead, we need these
target_compile_definitions(beyond-rgb-backend PRIVATE
    ASIO_STANDALONE
    WEBSOCKETPP_NO_BOOST
    _WEBSOCKETPP_CPP11_STL_
)
# VCPKG provided target_link_libraries
target_link_libraries(beyond-rgb-backend PRIVATE asio asio::asio)
target_link_libraries(beyond-rgb-backend PRIVATE jsoncons)
target_include_directories(beyond-rgb-backend PRIVATE ${CPPCODEC_INCLUDE_DIRS})
target_link_libraries(beyond-rgb-backend PRIVATE ${OpenCV_LIBS})

target_include_directories(beyond-rgb-backend PRIVATE ${TIFF_INCLUDE_DIR})
target_link_libraries(beyond-rgb-backend PRIVATE ${TIFF_LIBRARIES})

target_compile_definitions(beyond-rgb-backend PRIVATE ${LibRaw_r_DEFINITIONS})
target_include_directories(beyond-rgb-backend PRIVATE ${LibRaw_INCLUDE_DIR})
target_link_libraries(beyond-rgb-backend PRIVATE ${LibRaw_r_LIBRARIES} lcms::lcms)

target_link_libraries(beyond-rgb-backend PRIVATE PNG::PNG)

# Pthreads & OpenMP, only required on Linux & Windows since macOS includes these in its compiler.
if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx" AND NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-osx")
    target_link_libraries(beyond-rgb-backend PRIVATE Threads::Threads)
    target_link_libraries(beyond-rgb-backend PRIVATE OpenMP::OpenMP_CXX)
endif()

# Set C++ standard
set_property(TARGET beyond-rgb-backend PROPERTY CXX_STANDARD 20)

# weird fix for linus to remove malformed macro included in lib dependency
# TODO: remove this with a better fix
if (UNIX AND NOT APPLE)
    get_target_property(current_defs beyond-rgb-backend COMPILE_DEFINITIONS)
    if(current_defs)
        list(FILTER current_defs EXCLUDE REGEX "^-pthread$")
        set_target_properties(beyond-rgb-backend PROPERTIES COMPILE_DEFINITIONS "${current_defs}")
    endif()
endif()
