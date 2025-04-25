#pragma once

#include <utility>

constexpr int thread_count_idx = 1;
constexpr int points_idx = 2;
constexpr int default_points = 10000000;

std::pair<int, int> get_params(int argc, const char* argv[]);

