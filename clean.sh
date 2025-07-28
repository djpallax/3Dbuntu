#!/bin/bash
set -e

echo "Cleaning build files..."

# Borrar carpeta de build
rm -rf build

# Borrar archivos CMake generados en raíz
rm -f CMakeCache.txt
rm -rf CMakeFiles
rm -f Makefile
rm -f cmake_install.cmake

# Borrar archivos de traducción compilados
find . -name "*.qm" -delete

echo "Clean complete."
