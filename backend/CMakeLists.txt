cmake_minimum_required(VERSION 3.16)

# Sets the CMAKE Toolchain to the vcpkg installed using ./arch_config_environment
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# VCPKG on macOS requires system architectures to be set.
# Only Apple Silicon (arm64) is supported - Intel Macs are no longer supported.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin" AND NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "macOS architecture (Apple Silicon only)" FORCE)
    message(STATUS "macOS: Building for Apple Silicon (arm64)")
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
find_package(asio CONFIG QUIET)
if(NOT asio_FOUND)
    message(STATUS "Standalone ASIO not found, falling back to boost-asio")
    find_package(boost_asio CONFIG REQUIRED)
    set(ASIO_FALLBACK_TO_BOOST ON)
else()
    message(STATUS "Using standalone ASIO")
    set(ASIO_FALLBACK_TO_BOOST OFF)
endif()
find_package(TIFF REQUIRED)
find_package(jsoncons CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

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
if(ASIO_FALLBACK_TO_BOOST)
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::asio)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio)
endif()
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
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Testing Support
option(ENABLE_TESTS "Enable building unit tests" OFF)

if(ENABLE_TESTS)
    enable_testing()
    find_package(Catch2 3 CONFIG REQUIRED)
    
    # Create test executable
    add_executable(unit_tests
        test/unit/test_main.cpp
        test/unit/test_color_conversions.cpp
        test/unit/test_matrix_utils.cpp
        test/unit/test_csv_parser.cpp
        
        # Include necessary source files for testing
        src/utils/color_conversions.cpp
        src/utils/csv_parser.cpp
        src/reference_data/white_points.cpp
        src/reference_data/standard_observer.cpp
        src/reference_data/illuminants.cpp
        src/reference_data/ref_data.cpp
        src/reference_data/ref_data_array.cpp
        src/reference_data/color_patch.cpp
    )
    
    # Link test executable with Catch2 and project sources
    target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)
    target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
    # Link the same libraries as main project
    if(ASIO_FALLBACK_TO_BOOST)
        target_link_libraries(unit_tests PRIVATE Boost::asio)
    else()
        target_link_libraries(unit_tests PRIVATE asio::asio)
    endif()
    target_link_libraries(unit_tests PRIVATE jsoncons)
    target_link_libraries(unit_tests PRIVATE ${OpenCV_LIBS})
    target_link_libraries(unit_tests PRIVATE ${TIFF_LIBRARIES})
    target_link_libraries(unit_tests PRIVATE ${LibRaw_r_LIBRARIES} lcms::lcms)
    target_link_libraries(unit_tests PRIVATE PNG::PNG)
    
    # Platform-specific libs for tests
    if(NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "x64-osx" AND NOT ${VCPKG_TARGET_TRIPLET} STREQUAL "arm64-osx")
        target_link_libraries(unit_tests PRIVATE Threads::Threads)
        target_link_libraries(unit_tests PRIVATE OpenMP::OpenMP_CXX)
    endif()
    
    # Register tests with CTest
    include(CTest)
    include(Catch)
    catch_discover_tests(unit_tests)
    
    message(STATUS "Unit tests enabled")
endif()

# Code Coverage Support (optional, enabled with -DENABLE_COVERAGE=ON)
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage -O0 -g)
        target_link_options(${PROJECT_NAME} PRIVATE --coverage)
    else()
        message(WARNING "Code coverage not supported for this compiler")
    endif()
endif()

# weird fix for linux to remove malformed macro included in lib dependency
# TODO: remove this with a better fix
if (UNIX AND NOT APPLE)
    get_target_property(current_defs ${PROJECT_NAME} COMPILE_DEFINITIONS)
    if(current_defs)
        list(FILTER current_defs EXCLUDE REGEX "^-pthread$")
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "${current_defs}")
    endif()
endif()
