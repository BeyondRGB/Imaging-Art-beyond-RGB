# FindIccDEV.cmake — builds IccProfLib2 from iccDEV submodule

set(ICCDEV_SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules/iccDEV")

if (NOT EXISTS "${ICCDEV_SUBMODULE_DIR}/IccProfLib/IccProfile.h")
    message(FATAL_ERROR
            "iccDEV submodule not found at ${ICCDEV_SUBMODULE_DIR}\n"
            "Run: git submodule update --init --recursive"
    )
endif ()

# Variables expected by iccDEV's CMakeLists
set(ENABLE_SHARED_LIBS ON CACHE BOOL "Build shared iccDEV libs" FORCE)
set(ENABLE_STATIC_LIBS OFF CACHE BOOL "Build static iccDEV libs" FORCE)
set(ENABLE_INSTALL_RIM OFF CACHE BOOL "Skip iccDEV install targets" FORCE)

set(PROJECT_UP_NAME "REFICCMAX")
set(REFICCMAX_VERSION "2.3.1.5")
set(REFICCMAX_MAJOR_VERSION "2")

add_subdirectory(
        "${ICCDEV_SUBMODULE_DIR}/Build/Cmake/IccProfLib"
        "${CMAKE_CURRENT_BINARY_DIR}/IccProfLib"
)

# Fix missing PUBLIC include propagation in upstream CMake
target_include_directories(IccProfLib2 PUBLIC
        # Include the entire iccDEV as a header search path. This allows us to include <IccProfLib/Header> instead of the headers being at the top level
        "${ICCDEV_SUBMODULE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/IccProfLib"
)

message(STATUS "iccDEV IccProfLib2 ready (${REFICCMAX_VERSION})")
