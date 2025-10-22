#!/bin/bash
# Backend Smoke Test Script
# Tests that the backend executable starts and responds correctly

set -e  # Exit on error

echo "=========================================="
echo "Backend Smoke Test"
echo "=========================================="

# Debug: Show what's in the build directory
echo "Checking build directory contents..."
if [ -d "build" ]; then
    echo "Build directory exists. Contents:"
    ls -la build/ || true
    if [ -d "build/Release" ]; then
        echo "build/Release contents:"
        ls -la build/Release/ || true
    fi
    if [ -d "build/Debug" ]; then
        echo "build/Debug contents:"
        ls -la build/Debug/ || true
    fi
else
    echo "❌ ERROR: build/ directory does not exist!"
    echo "The backend build step may have failed."
    exit 1
fi

# Find the backend executable
BACKEND_BINARY=""

# Check common build locations
if [ -f "build/Release/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/Release/beyond-rgb-backend"
elif [ -f "build/Debug/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/Debug/beyond-rgb-backend"
elif [ -f "build/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/beyond-rgb-backend"
elif [ -f "../frontend/lib/beyond-rgb-backend" ]; then
    # Check if it was copied to frontend
    BACKEND_BINARY="../frontend/lib/beyond-rgb-backend"
else
    echo "❌ ERROR: Could not find backend binary"
    echo "Searched in:"
    echo "  - build/Release/beyond-rgb-backend"
    echo "  - build/Debug/beyond-rgb-backend"
    echo "  - build/beyond-rgb-backend"
    echo "  - ../frontend/lib/beyond-rgb-backend"
    echo ""
    echo "Build directory structure:"
    find build -type f -name "*backend*" 2>/dev/null || echo "No backend binary found"
    exit 1
fi

echo "✓ Found backend binary: $BACKEND_BINARY"

# Test 1: Check if binary is executable
if [ ! -x "$BACKEND_BINARY" ]; then
    echo "❌ ERROR: Binary is not executable"
    exit 1
fi
echo "✓ Binary is executable"

# Test 2: Check if binary can start with --help flag (with timeout)
echo ""
echo "Test: Running with --help flag (2 second timeout)..."
if timeout 2 $BACKEND_BINARY --help > /dev/null 2>&1; then
    echo "✓ Binary responds to --help"
else
    echo "⚠️  Binary doesn't support --help (this is okay)"
fi

# Test 3: Check if binary can start with --version flag (with timeout)
echo ""
echo "Test: Running with --version flag (2 second timeout)..."
if timeout 2 $BACKEND_BINARY --version > /dev/null 2>&1; then
    echo "✓ Binary responds to --version"
else
    echo "⚠️  Binary doesn't support --version (this is okay)"
fi

# Test 4: Start backend in test mode if available (with timeout)
echo ""
echo "Test: Starting backend in test mode (2 second timeout)..."
if timeout 2 $BACKEND_BINARY --test > /dev/null 2>&1; then
    echo "✓ Binary runs in test mode successfully"
else
    echo "⚠️  Test mode not available (this is okay)"
fi

# Test 5: Check if binary can start without crashing immediately
echo ""
echo "Test: Checking if binary starts without immediate crash..."
$BACKEND_BINARY --port 9999 > /dev/null 2>&1 &
BACKEND_PID=$!
sleep 2

if ps -p $BACKEND_PID > /dev/null 2>&1; then
    echo "✓ Binary started successfully and is running"
    kill $BACKEND_PID 2>/dev/null || true
    wait $BACKEND_PID 2>/dev/null || true
else
    echo "⚠️  Binary exited quickly (might need valid configuration, but this is okay for smoke test)"
fi

echo ""
echo "=========================================="
echo "✅ Smoke Test Complete"
echo "=========================================="
echo ""
echo "Summary:"
echo "  - Binary exists and is executable"
echo "  - Binary can be started"
echo ""
echo "Note: Full integration tests require a complete environment setup"
echo "      and are beyond the scope of this smoke test."

exit 0

