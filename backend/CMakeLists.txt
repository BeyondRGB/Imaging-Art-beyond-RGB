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
#set(VCPKG_TRACE_FIND_PACKAGE ON)

project(beyond-rgb-backend DESCRIPTION "Imaging Art Beyond RGB" VERSION 2.2.0)

configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_SOURCE_DIR}/include/version.h
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

set(PNG_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/png")
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

# Create .exe
add_executable(${PROJECT_NAME})

# Find all Header and Source files
target_sources(${PROJECT_NAME} PRIVATE
        src/main.cpp

        # backend_process
        src/backend_process/backend_process.cpp
        src/backend_process/ColorManagedImage.cpp
        src/backend_process/HalfSizePreview.cpp
        src/backend_process/pipeline.cpp
        src/backend_process/ReportRequest.cpp
        src/backend_process/SpectralPicker.cpp
        src/backend_process/SpectralPickerMeasured.cpp
        src/backend_process/ThumbnailLoader.cpp

        # image_processing
        src/image_processing/BitDepthScaler.cpp
        src/image_processing/ChannelSelector.cpp
        src/image_processing/ColorManagedCalibrator.cpp
        src/image_processing/FlatFieldor.cpp
        src/image_processing/ImageCalibrator.cpp
        src/image_processing/ImageProcessor.cpp
        src/image_processing/ImageReader.cpp
        src/image_processing/NoiseReduction.cpp
        src/image_processing/PixelRegestor.cpp
        src/image_processing/PreProcessor.cpp
        src/image_processing/ResultsProcessor.cpp
        src/image_processing/SpectralCalibrator.cpp
        src/image_processing/Verification.cpp

        # image_processing / results
        src/image_processing/results/calibration_results.cpp
        src/image_processing/results/colorimetry_formater.cpp
        src/image_processing/results/general_info_formater.cpp
        src/image_processing/results/m_color_formater.cpp
        src/image_processing/results/m_spectral_formater.cpp
        src/image_processing/results/r_camera_formater.cpp

        # image_util
        src/image_util/ArtObject.cpp
        src/image_util/BitDepthFinder.cpp
        src/image_util/ColorProfiles.cpp
        src/image_util/ColorTarget.cpp
        src/image_util/Image.cpp

        # image_util / image_reader
        src/image_util/image_reader/LibRawReader.cpp
        src/image_util/image_reader/LibRawThumbnail.cpp
        src/image_util/image_reader/LibTiffReader.cpp
        src/image_util/image_reader/TiffReaderOpenCV.cpp

        # image_util / image_writer
        src/image_util/image_writer/ImageWriter.cpp
        src/image_util/image_writer/ImageWriterStrategy.cpp
        src/image_util/image_writer/LibpngWriter.cpp
        src/image_util/image_writer/LibTiffWriter.cpp

        # reference_data
        src/reference_data/color_patch.cpp
        src/reference_data/illuminants.cpp
        src/reference_data/ref_data.cpp
        src/reference_data/ref_data_array.cpp
        src/reference_data/standard_observer.cpp
        src/reference_data/white_points.cpp

        # server
        src/server/communication_obj.cpp
        src/server/communicator.cpp
        src/server/globals_singleton.cpp
        src/server/process_manager.cpp
        src/server/request_server.cpp

        # utils
        src/utils/calibration_util.cpp
        src/utils/cmd_arg_manager.cpp
        src/utils/color_conversions.cpp
        src/utils/csv_parser.cpp
        src/utils/general_utils.cpp
        src/utils/json.cpp
        src/utils/jsonafiable.cpp
        src/utils/time_tracker.cpp
)

# Set all source include files (ie. *.h)
target_include_directories(${PROJECT_NAME} PUBLIC "include/")

# adds stacktraces to unix machines
if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-rdynamic")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14)
        target_link_libraries(${PROJECT_NAME} PRIVATE stdc++exp)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE stdc++_libbacktrace)
    endif()
endif()

# adds websocketpp as a target lib for compilation

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/submodules/websocketpp
)
# adds extra modes for compilation, since we dont use boost and asio instead, we need these
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASIO_STANDALONE
    WEBSOCKETPP_NO_BOOST
    _WEBSOCKETPP_CPP11_STL_
)
# VCPKG provided target_link_libraries
target_link_libraries(${PROJECT_NAME} PRIVATE asio asio::asio)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncons)
target_include_directories(${PROJECT_NAME} PRIVATE ${CPPCODEC_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

target_include_directories(${PROJECT_NAME} PRIVATE ${TIFF_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TIFF_LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PRIVATE ${LibRaw_r_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE ${LibRaw_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LibRaw_r_LIBRARIES} lcms::lcms)

target_link_libraries(${PROJECT_NAME} PRIVATE PNG::PNG)

# Pthreads & OpenMP, only required on Linux & Windows since macOS includes these in its compiler.
if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx" AND NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-osx")
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# Set C++ standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

# weird fix for linux to remove malformed macro included in lib dependency
# TODO: remove this with a better fix
if (UNIX AND NOT APPLE)
    get_target_property(current_defs ${PROJECT_NAME} COMPILE_DEFINITIONS)
    if(current_defs)
        list(FILTER current_defs EXCLUDE REGEX "^-pthread$")
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "${current_defs}")
    endif()
endif()
