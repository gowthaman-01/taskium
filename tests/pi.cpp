#include "pi.hpp"

#include "../core/logger.hpp"
#include "../include/thread_pool.hpp"

#include <future>
#include <iostream>
#include <random>
#include <thread>
#include <sstream>
#include <string>
#include <vector>

using Taskium::Logger;
using Taskium::ThreadPool;

Logger logger;

double run_pi_estimation(ThreadPool& thread_pool, int thread_count, int points) {
    logger.info("starting pi estimation with "
                + std::to_string(thread_count)
                + " threads and "
                + std::to_string(points)
                + " points per thread\n");
    
    auto futures = submit_pi_tasks(thread_pool, thread_count, points);
    return aggregate_pi_results(futures, thread_count, points);
}

std::vector<std::future<int>> submit_pi_tasks(ThreadPool& thread_pool, int thread_count, int points) {
    std::vector<std::future<int>> futures;
    
    for (int i = 0; i < thread_count; ++i) {
        // Create a promise-future pair for each task
        auto promise_ptr = std::make_shared<std::promise<int>>();
        std::future<int> future = promise_ptr->get_future();
        
        futures.push_back(std::move(future));
        
        logger.info("submitted pi estimation task "
                    + std::to_string(i));
        
        thread_pool.submit([i, points, promise_ptr]() {
            pi_task(i, points, std::move(*promise_ptr));
        });
    }
    
    return futures;
}

void pi_task(int id, int points, std::promise<int> promise) {
    std::ostringstream oss;
    
    oss << "[THREAD " << std::this_thread::get_id() << "] task " << id << " started";
    logger.info(oss.str());
    
    // Mersenne Twister RNG
    std::mt19937 rng(std::random_device{}());
    // Generates random floating-point numbers between 0 and 1
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int in_circle = 0;
    for (int i = 0; i < points; ++i) {
        // Generate x, y coordinates inside a unit square
        double x = dist(rng);
        double y = dist(rng);
        
        // Count how many points fall inside unit circle
        if ((x * x + y * y) <= 1.0) {
            ++in_circle;
        }
    }
    
    promise.set_value(in_circle);
    
    // Clear stream before reuse
    oss.str("");
    oss.clear();
    
    oss << "[THREAD " << std::this_thread::get_id() << "] task " << id << " completed";
    logger.info(oss.str());
}

double aggregate_pi_results(std::vector<std::future<int>>& futures, int thread_count, int points) {
    double total_points = points * thread_count;
    double total_in_circle = 0;
    
    for (auto& future: futures) {
        auto in_circle = future.get();
        total_in_circle += in_circle;
    }

    double pi_estimate = total_in_circle / total_points * 4.0;
    
    logger.general("\n[RESULT] estimated pi = "
                   + std::to_string(pi_estimate));
    
    return pi_estimate;
}

