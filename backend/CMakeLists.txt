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
FIND_PACKAGE( OpenCV REQUIRED )                              
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

# LCMS is needed for LibRAW. For some reason on 
# OSX and Linux, this dependency needs to be explicitly
# found and linked. LCMS was added in dependencies.txt.
find_package(lcms REQUIRED) # Little Color Management System
find_package(LibRaw REQUIRED)

# Pthreads & OpenMP on Windows & Linux
if( NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx")
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
add_executable(beyond-rgb-backend ${SOURCES})
#   Set all source include files (ie. *.h)
target_include_directories(beyond-rgb-backend PUBLIC ${headers})
target_include_directories(beyond-rgb-backend PUBLIC "src/")

#----------------------------------------------------
#	Add VCPKG provided target_link_libraties Here
#----------------------------------------------------
target_link_libraries(beyond-rgb-backend PRIVATE websocketpp::websocketpp)
target_link_libraries(beyond-rgb-backend PRIVATE asio asio::asio)
target_link_libraries(beyond-rgb-backend PRIVATE jsoncons)
target_include_directories(beyond-rgb-backend PRIVATE ${CPPCODEC_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES ( beyond-rgb-backend PRIVATE ${OpenCV_LIBS} )


target_include_directories(beyond-rgb-backend PRIVATE ${TIFF_INCLUDE_DIR}) 
target_link_libraries(beyond-rgb-backend PRIVATE ${TIFF_LIBRARIES})

target_compile_definitions(beyond-rgb-backend PRIVATE ${LibRaw_r_DEFINITIONS})
target_include_directories(beyond-rgb-backend PRIVATE ${LibRaw_INCLUDE_DIR})
target_link_libraries(beyond-rgb-backend PRIVATE ${LibRaw_r_LIBRARIES} lcms::lcms)

# LibPNG
if( ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows")
    target_link_libraries(beyond-rgb-backend PRIVATE png)
else()
    target_link_libraries(beyond-rgb-backend PRIVATE png_static)
endif()


# Pthreads & OpenMP on Windows & Linux
if( NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx")
    target_link_libraries(beyond-rgb-backend PRIVATE Threads::Threads)
    target_link_libraries(beyond-rgb-backend PRIVATE OpenMP::OpenMP_CXX)
endif()

# Need the c++ standard explicitly stated for compiling on OSX.
set_property(TARGET beyond-rgb-backend PROPERTY CXX_STANDARD 20)
