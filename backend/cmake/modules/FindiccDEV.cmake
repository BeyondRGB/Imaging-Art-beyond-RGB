# FindIccDEV.cmake — builds IccProfLib2 from iccDEV submodule

set(ICCDEV_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules/iccDEV")

if (NOT EXISTS "${ICCDEV_SUBMODULE_DIR}/IccProfLib/IccProfile.h")
    message(FATAL_ERROR
            "iccDEV submodule not found at ${ICCDEV_SUBMODULE_DIR}\n"
            "Run: git submodule update --init --recursive"
    )
endif ()

# Variables expected by iccDEV's CMakeLists
set(ENABLE_SHARED_LIBS OFF CACHE BOOL "Build shared iccDEV libs" FORCE)
set(ENABLE_STATIC_LIBS ON CACHE BOOL "Build static iccDEV libs" FORCE)
set(ENABLE_INSTALL_RIM OFF CACHE BOOL "Skip iccDEV install targets" FORCE)

# Derive iccDEV version from submodule by David Hoyt.
file(
        STRINGS
        "${ICCDEV_SUBMODULE_DIR}/IccProfLib/IccProfLibVer.h"
        ICCDEV_VERSION_LINE
        REGEX "^#define[ \t]+ICCPROFLIBVER[ \t]+\"[^\"]+\""
)
if (NOT ICCDEV_VERSION_LINE)
    message(FATAL_ERROR
            "Unable to extract ICCPROFLIBVER from ${ICCDEV_SUBMODULE_DIR}/IccProfLib/IccProfLibVer.h"
    )
endif ()
string(
        REGEX REPLACE
        ".*\"([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)\".*"
        "\\1"
        REFICCMAX_VERSION
        "${ICCDEV_VERSION_LINE}"
)

string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" REFICCMAX_MAJOR_VERSION "${REFICCMAX_VERSION}")

add_subdirectory(
        "${ICCDEV_SUBMODULE_DIR}/Build/Cmake/IccProfLib"
        "${CMAKE_CURRENT_BINARY_DIR}/IccProfLib"
)

# Fix missing PUBLIC include propagation in upstream CMake
target_include_directories(IccProfLib2-static PUBLIC
        # Include the entire iccDEV as a header search path. This allows us to include <IccProfLib/Header> instead of the headers being at the top level
        "${ICCDEV_SUBMODULE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/IccProfLib"
)

message(STATUS "iccDEV IccProfLib2-static ready (${REFICCMAX_VERSION})")
