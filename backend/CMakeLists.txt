cmake_minimum_required(VERSION 3.16)  
include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
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
find_package(libpng CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(PNG_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/png")
find_package(PNG REQUIRED)


# LCMS and LibRAW
find_package(lcms REQUIRED)
find_package(LibRaw REQUIRED)

# Pthreads & OpenMP
if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx")
    find_package(Threads REQUIRED)
    find_package(OpenMP REQUIRED)
endif()

# add these AFTER vcpkg dependencies are added
set(BUILD_TESTS OFF CACHE BOOL "Disable tests in websocketpp" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples in websocketpp" FORCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/submodules/websocketpp)

# Find all Header and Source files
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Create .exe
add_executable(beyond-rgb-backend ${SOURCES})

# Set all source include files (ie. *.h)
target_include_directories(beyond-rgb-backend PUBLIC "src/")

# adds websocketpp as a target lib for compilation
target_include_directories(beyond-rgb-backend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/submodules/websocketpp
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

# LibPNG linking
if(${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows")
    #target_link_libraries(beyond-rgb-backend PRIVATE png)
    target_link_libraries(beyond-rgb-backend PRIVATE PNG::PNG)
else()
    target_link_libraries(beyond-rgb-backend PRIVATE png_static)
endif()

# Pthreads & OpenMP
if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx")
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
