cmake_minimum_required(VERSION 3.16)  
include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(ArchitecutreTest DESCRIPTION "A Test project" VERSION 0.0.1)


#------------------------------------------
#	Add VCPKG provided find_package Here
#------------------------------------------
find_package(websocketpp CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(TIFF REQUIRED)
find_package(jsoncons CONFIG REQUIRED)
find_package(libpng CONFIG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

# LCMS is needed for LibRAW. For some reason on 
# OSX and Linux, this dependency needs to be explicitly
# found and linked. LCMS was added in dependencies.txt.
find_package(lcms REQUIRED) # Little Color Management System
find_package(LibRaw REQUIRED)

# Pthreads & OpenMP on Linux
if( ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-linux")
    find_package(Threads REQUIRED)
    find_package(OpenMP REQUIRED)
endif()

#   Find all Header and Source files
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

#   Print out all Header and Source files found
message(STATUS "Headers: ${HEADERS}")
message(STATUS "Source Files: ${SOURCES}")
#   Create .exe
add_executable(app ${SOURCES})
#   Set all source include files (ie. *.h)
target_include_directories(app PUBLIC ${headers})
target_include_directories(app PUBLIC "src/")

#----------------------------------------------------
#	Add VCPKG provided target_link_libraties Here
#----------------------------------------------------
target_link_libraries(app PRIVATE websocketpp::websocketpp)
target_link_libraries(app PRIVATE asio asio::asio)
target_link_libraries(app PRIVATE jsoncons)
target_include_directories(app PRIVATE ${CPPCODEC_INCLUDE_DIRS})

target_include_directories(app PRIVATE ${TIFF_INCLUDE_DIR}) 
target_link_libraries(app PRIVATE ${TIFF_LIBRARIES})

target_compile_definitions(app PRIVATE ${LibRaw_r_DEFINITIONS})
target_include_directories(app PRIVATE ${LibRaw_INCLUDE_DIR})
target_link_libraries(app PRIVATE ${LibRaw_r_LIBRARIES} lcms::lcms)

# LibPNG
if( ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows")
    target_link_libraries(app PRIVATE png)
else()
    target_link_libraries(app PRIVATE png_static)
endif()


# Pthreads & OpenMP on Linux
if( ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-linux")
    target_link_libraries(app PRIVATE Threads::Threads)
    target_link_libraries(app PRIVATE OpenMP::OpenMP_CXX)
endif()

# Need the c++ standard explicitly stated for compiling on OSX.
set_property(TARGET app PROPERTY CXX_STANDARD 20)