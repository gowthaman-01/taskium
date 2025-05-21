#include "cli_config.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

std::pair<int, int> get_params(int argc, const char* argv[]) {
    // Default values
    int thread_count = std::thread::hardware_concurrency();     // Dynamically detects number of CPU cores
    int points = default_point_count;                           // 10 million
    
    // CLI overrides
    try {
        if (argc >= 2) {
            thread_count = std::stoi(argv[thread_count_idx]);
        }
        
        if (argc >= 3) {
            points = std::stoi(argv[points_idx]);
        }
    }  catch (const std::exception& e) {
        std::cerr << "[WARN] Invalid CLI arguments detected!\n"
                  << "Falling back to defaults:\nthread_count = 4, points = 10,000,000\n";
    }
    
    return {thread_count, points};
}

