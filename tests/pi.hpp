#pragma once

#include "../include/thread_pool.hpp"

#include <future>

using Taskium::ThreadPool;

/**
 * @brief Runs the full Pi estimation process using a ThreadPool.
 *
 * @param thread_pool Reference to the ThreadPool for task execution.
 * @param thread_count Number of tasks to submit (one per thread).
 * @param points Number of random points per task (per thread).
 * @return Estimated value of Pi.
 */
double run_pi_estimation(ThreadPool& thread_pool, int thread_count, int points);

/**
 * @brief Submits Monte Carlo Pi estimation tasks to the ThreadPool.
 *
 * @param thread_pool Reference to the ThreadPool for task execution.
 * @param thread_count Number of tasks to submit.
 * @param points Number of random points per task.
 * @return Vector of futures corresponding to each task's result.
 */
std::vector<std::future<int>> submit_pi_tasks(ThreadPool& thread_pool, int thread_count, int points);

/**
 * @brief Performs Monte Carlo Pi estimation for a given number of points.
 *
 * This function generates random (x, y) points within a unit square and counts
 * how many fall inside the unit circle. The result (in-circle count) is set via the provided promise.
 *
 * @param id ID of the current task
 * @param points Number of random points to generate.
 * @param promise Promise used to set the in-circle count result.
 */
void pi_task(int id, int points, std::promise<int> promise);

/**
 * @brief Aggregates results from all futures and computes the Pi estimate.
 *
 * @param futures Vector of futures from submitted tasks.
 * @param thread_count Number of tasks (threads).
 * @param points Number of points per task.
 * @return Estimated value of Pi.
 */
double aggregate_pi_results(std::vector<std::future<int>>& futures, int thread_count, int points);

