cmake_minimum_required(VERSION 3.16)  
include(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
project(ArchitecutreTest DESCRIPTION "A Test project" VERSION 0.0.1)


#------------------------------------------
#	Add VCPKG provided find_package Here
#------------------------------------------
find_package(websocketpp CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(LibRaw REQUIRED)
# find_package(tiff REQUIRED)
find_path(TIFF_INCLUDE_DIR tiffio.h)
find_library(TIFF_LIBRARY tiff)

#   Find all Header and Source files
#file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
#file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/ex1.cpp")

#   Print out all Header and Source files found
message(STATUS "Headers: ${HEADERS}")
message(STATUS "Source Files: ${SOURCES}")
#   Create .exe
add_executable(app ${SOURCES})
#   Set all source include files (ie. *.h)
target_include_directories(app PUBLIC ${headers})

#----------------------------------------------------
#	Add VCPKG provided target_link_libraties Here
#----------------------------------------------------
target_link_libraries(app PRIVATE websocketpp::websocketpp)
target_link_libraries(app PRIVATE asio asio::asio)

target_include_directories(app PRIVATE ${TIFF_INCLUDE_DIR}) 
target_link_libraries(app PRIVATE ${TIFF_LIBRARY})

target_compile_definitions(app PRIVATE ${LibRaw_r_DEFINITIONS})
target_include_directories(app PRIVATE ${LibRaw_INCLUDE_DIR})
target_link_libraries(app PRIVATE ${LibRaw_r_LIBRARIES})



# Need the c++ standard explicitly stated for compiling on OSX.
set_property(TARGET app PROPERTY CXX_STANDARD 11)