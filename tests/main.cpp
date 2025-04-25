#include "cli_config.hpp"
#include "pi.hpp"

#include "../core/logger.hpp"
#include "../include/thread_pool.hpp"

#include <iostream>
#include <memory>
#include <thread>
#include <string>
#include <utility>
#include <vector>

using Taskium::Logger;
using Taskium::ThreadPool;

int main(int argc, const char* argv[]) {
    auto [thread_count, points] = get_params(argc, argv);
    ThreadPool thread_pool(thread_count);
    
    run_pi_estimation(thread_pool, thread_count, points);

    thread_pool.shutdown();
}
