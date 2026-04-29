#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
cmake --build . -- -j$(nproc)

echo "Build complete. Run: $BUILD_DIR/codespace_example"
echo "Run tests: $BUILD_DIR/codespace_test"
echo "Run benchmark: $BUILD_DIR/codespace_benchmark"
