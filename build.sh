#!/bin/bash
set -euo pipefail

BUILD_TYPE=${1:-Release}
CMAKE_EXTRA_ARGS=${2:-""}

# Reset build directory
rm -rf build
mkdir -p build && cd build

# Configure
cmake -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-13 -DCMAKE_BUILD_TYPE="$BUILD_TYPE" $CMAKE_EXTRA_ARGS ..

# Build
if [[ "$OSTYPE" == "darwin"* ]]; then
  # macOS
  NUM_PROCESSORS="$(sysctl -n hw.ncpu)"
else
  NUM_PROCESSORS="$(nproc)"
fi
cmake --build . --config "$BUILD_TYPE" -- -j "$NUM_PROCESSORS"
