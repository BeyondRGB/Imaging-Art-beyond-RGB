# - Config file for the iccDEV package
# It defines the following variables
#  iccDEV_INCLUDE_DIRS - include directories for iccDEV
#  iccDEV_LIBRARIES    - libraries to link against
#  iccDEV_EXECUTABLE   - the bar executable

# Compute paths
get_filename_component(iccDEV_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(iccDEV_INCLUDE_DIRS "@CONF_INCLUDE_DIRS@")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET iccDEV AND NOT iccDEV_BINARY_DIR)
  include("${iccDEV_CMAKE_DIR}/iccDEVConfig.cmake")
endif()

# These are IMPORTED targets created by iccDEVLibTargets.cmake
set(iccDEV_LIBRARIES iccDEV)
