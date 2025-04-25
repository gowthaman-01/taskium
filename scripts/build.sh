#!/bin/bash
# Script to build locally

set -e # Exit immediately if a command fails

# Go to the root directory of the project
PROJECT_DIR="$(dirname "$0")/.."
cd "$PROJECT_DIR"

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
  echo "================================="
  echo "Creating build directory..."
  echo "================================="
  mkdir build
  echo
fi

# Change to the build directory
cd build

# Run CMake to configure the project
echo "================================="
echo "Running CMake..."
echo "================================="
cmake ..
echo

# Build the project
echo "================================="
echo "Building the project..."
echo "================================="
cmake --build .
echo

echo "================================="
echo "Project built successfully."
echo "================================="
