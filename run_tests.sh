#!/bin/sh

# Test runner script for Unix/Mac
# Runs backend Catch2 tests, frontend Cypress tests, and collects code coverage

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BACKEND_DIR="${SCRIPT_DIR}/backend"
FRONTEND_DIR="${SCRIPT_DIR}/frontend"
COVERAGE_DIR="${SCRIPT_DIR}/coverage"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Cleanup function
cleanup() {
    echo -e "\n${YELLOW}Cleaning up...${NC}"
}

# Trap to ensure cleanup on exit
trap cleanup EXIT INT TERM

echo -e "${GREEN}=== Running All Tests ===${NC}\n"

# Create coverage directory
mkdir -p "${COVERAGE_DIR}"

# Determine platform
if [ "$(uname)" = "Darwin" ]; then
    PLATFORM="macos"
    BUILD_SCRIPT="osx_build.sh"
    VCPKG_TRIPLET="x64-osx"
    # Check architecture
    if [ "$(uname -m)" = "arm64" ]; then
        VCPKG_TRIPLET="arm64-osx"
    fi
else
    PLATFORM="linux"
    BUILD_SCRIPT="linux_build.sh"
    VCPKG_TRIPLET="x64-linux"
fi

# Step 1: Build backend with tests enabled
echo -e "${GREEN}[1/6] Building backend with tests enabled...${NC}"
cd "${BACKEND_DIR}"

# Check if vcpkg is configured
if [ ! -d "vcpkg/installed" ]; then
    echo -e "${YELLOW}Warning: vcpkg not configured. Running config script...${NC}"
    ./unix_config_environment.sh
fi

# Build with tests and coverage flags
BUILD_MODE="Debug"
BUILD_DIR="build/${BUILD_MODE}"

echo "Running CMake with tests enabled..."
cmake -B "${BUILD_DIR}" -S . \
    -D CMAKE_BUILD_TYPE=${BUILD_MODE} \
    -D ENABLE_TESTS=ON \
    -D CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage" \
    -D CMAKE_EXE_LINKER_FLAGS="${CMAKE_EXE_LINKER_FLAGS} --coverage" \
    -D VCPKG_TARGET_TRIPLET="${VCPKG_TRIPLET}"

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to configure CMake${NC}"
    exit 1
fi

echo "Building backend..."
cmake --build "${BUILD_DIR}"

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to build backend${NC}"
    exit 1
fi

# Step 2: Run backend tests
echo -e "\n${GREEN}[2/6] Running backend Catch2 tests...${NC}"
cd "${BUILD_DIR}"

if [ ! -f "unit_tests" ]; then
    echo -e "${RED}Error: unit_tests executable not found${NC}"
    exit 1
fi

./unit_tests
BACKEND_TEST_RESULT=$?

if [ $BACKEND_TEST_RESULT -ne 0 ]; then
    echo -e "${RED}Backend tests failed${NC}"
    exit 1
fi

echo -e "${GREEN}Backend tests passed!${NC}"

# Step 3: Generate backend coverage report
echo -e "\n${GREEN}[3/6] Generating backend coverage report...${NC}"
mkdir -p "${COVERAGE_DIR}/backend"

# Collect coverage data using lcov (if available) or gcov
if command -v lcov >/dev/null 2>&1; then
    lcov --capture --directory . --output-file "${COVERAGE_DIR}/backend/coverage.info" || true
    lcov --remove "${COVERAGE_DIR}/backend/coverage.info" \
        '/usr/*' \
        '*/vcpkg/*' \
        '*/submodules/*' \
        '*/test/*' \
        --output-file "${COVERAGE_DIR}/backend/coverage.info" || true
    genhtml "${COVERAGE_DIR}/backend/coverage.info" --output-directory "${COVERAGE_DIR}/backend/html" || true
    echo -e "${GREEN}Backend coverage report generated at ${COVERAGE_DIR}/backend/html/index.html${NC}"
else
    echo -e "${YELLOW}Warning: lcov not found. Install lcov for coverage reports.${NC}"
fi

# Step 4: Install frontend dependencies
echo -e "\n${GREEN}[4/6] Setting up frontend and running frontend tests...${NC}"
cd "${FRONTEND_DIR}"

# Install dependencies if needed
if [ ! -d "node_modules" ]; then
    echo "Installing frontend dependencies..."
    npm install
fi

# Step 5: Run frontend tests
echo -e "\n${GREEN}[5/6] Running frontend unit and Cypress tests...${NC}"
npm run test:coverage
FRONTEND_TEST_RESULT=$?

if [ $FRONTEND_TEST_RESULT -ne 0 ]; then
    echo -e "${RED}Frontend tests failed${NC}"
    exit 1
fi

echo -e "${GREEN}Frontend tests passed!${NC}"

# Step 6: Generate frontend coverage report (if coverage plugin is configured)
if [ -d "${FRONTEND_DIR}/coverage" ]; then
    echo -e "\n${GREEN}[6/6] Frontend coverage report available at ${FRONTEND_DIR}/coverage${NC}"
    mkdir -p "${COVERAGE_DIR}/frontend"
    cp -r "${FRONTEND_DIR}/coverage"/* "${COVERAGE_DIR}/frontend/" 2>/dev/null || true
fi

# Summary
echo -e "\n${GREEN}=== All Tests Passed! ===${NC}"
echo -e "Backend coverage: ${COVERAGE_DIR}/backend/html/index.html"
echo -e "Frontend coverage: ${COVERAGE_DIR}/frontend (if available)"

exit 0
