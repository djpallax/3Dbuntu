#!/bin/bash
set -e

echo "Building 3Dbuntu..."

mkdir -p build
cd build

cmake ..
make -j$(nproc)

echo "Running..."
./3Dbuntu
