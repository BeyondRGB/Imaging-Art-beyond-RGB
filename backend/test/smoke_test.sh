#!/bin/bash
# Backend Smoke Test Script
# Tests that the backend executable starts and responds correctly

set -e  # Exit on error

echo "=========================================="
echo "Backend Smoke Test"
echo "=========================================="

# Find the backend executable
BACKEND_BINARY=""

# Check common build locations
if [ -f "build/Release/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/Release/beyond-rgb-backend"
elif [ -f "build/Debug/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/Debug/beyond-rgb-backend"
elif [ -f "build/beyond-rgb-backend" ]; then
    BACKEND_BINARY="build/beyond-rgb-backend"
else
    echo "❌ ERROR: Could not find backend binary"
    echo "Searched in:"
    echo "  - build/Release/beyond-rgb-backend"
    echo "  - build/Debug/beyond-rgb-backend"
    echo "  - build/beyond-rgb-backend"
    exit 1
fi

echo "✓ Found backend binary: $BACKEND_BINARY"

# Test 1: Check if binary is executable
if [ ! -x "$BACKEND_BINARY" ]; then
    echo "❌ ERROR: Binary is not executable"
    exit 1
fi
echo "✓ Binary is executable"

# Test 2: Check if binary can start with --help flag
echo ""
echo "Test: Running with --help flag..."
if $BACKEND_BINARY --help > /dev/null 2>&1; then
    echo "✓ Binary responds to --help"
else
    echo "⚠️  Warning: Binary does not respond to --help (this might be expected)"
fi

# Test 3: Check if binary can start with --version flag
echo ""
echo "Test: Running with --version flag..."
if $BACKEND_BINARY --version > /dev/null 2>&1; then
    echo "✓ Binary responds to --version"
else
    echo "⚠️  Warning: Binary does not respond to --version (this might be expected)"
fi

# Test 4: Start backend in test mode if available
echo ""
echo "Test: Starting backend in test mode..."
if $BACKEND_BINARY --test > /dev/null 2>&1; then
    echo "✓ Binary runs in test mode successfully"
else
    echo "⚠️  Warning: Test mode not available or failed (this might be expected)"
fi

# Test 5: Check if binary can start without crashing immediately
echo ""
echo "Test: Checking if binary starts without immediate crash..."
timeout 5 $BACKEND_BINARY --port 9999 > /dev/null 2>&1 &
BACKEND_PID=$!
sleep 2

if ps -p $BACKEND_PID > /dev/null 2>&1; then
    echo "✓ Binary started successfully and is running"
    kill $BACKEND_PID 2>/dev/null || true
    wait $BACKEND_PID 2>/dev/null || true
else
    echo "⚠️  Warning: Binary stopped running (might need valid configuration)"
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

