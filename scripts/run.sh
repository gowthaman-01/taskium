#!/bin/bash

echo "================================="
echo "Taskium: Monte Carlo Pi Estimator"
echo "================================="

# Detect max hardware threads
if command -v nproc &> /dev/null; then
  MAX_THREADS=$(nproc)
else
  MAX_THREADS=$(sysctl -n hw.ncpu)
fi

# Explain arguments
echo "This program estimates Pi using multiple threads."
echo "You need to provide:"
echo "  1. Number of threads (1 to $MAX_THREADS)"
echo "  2. Number of points per thread (positive integer)"
echo ""

# Prompt for thread count with validation
while true; do
  read -p "Enter the number of threads (1-$MAX_THREADS): " THREAD_COUNT
  if [[ "$THREAD_COUNT" =~ ^[0-9]+$ ]] && [ "$THREAD_COUNT" -ge 1 ] && [ "$THREAD_COUNT" -le "$MAX_THREADS" ]; then
    break
  else
    echo "[WARN] Invalid input. Please enter a number between 1 and $MAX_THREADS."
  fi
done

MAX_INT=2147483647

# Prompt for points per thread with validation
while true; do
  read -p "Enter the number of points per thread (1-$MAX_INT): " POINTS_PER_THREAD
  if [[ "$POINTS_PER_THREAD" =~ ^[0-9]+$ ]] && [ "$POINTS_PER_THREAD" -gt 0 ] && [ "$POINTS_PER_THREAD" -le "$MAX_INT" ]; then
    break
  else
    echo "[WARN] Invalid input. Please enter a positive integer (max $MAX_INT)."
  fi
done

echo ""

cd build || exit 1
./taskium "$THREAD_COUNT" "$POINTS_PER_THREAD"

