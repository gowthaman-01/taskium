
# Taskium: Thread Pool Scheduler

![Taskium](https://github.com/user-attachments/assets/0846937c-28b1-46b0-8323-49d6afaccc66)

Taskium is a modern C++ thread pool framework designed to manage concurrent task execution.
It features lock-based scheduling, mutex-guarded queues, and condition-variable signaling to ensure safe, efficient task dispatch across multiple worker threads.

## Features

- **Lock-based thread pool** using **mutexes** and **condition variables**.
- **RAII-managed worker lifecycles** to ensure safe thread joins and clean shutdowns.
- **Concurrent task dispatch**: Supports submitting arbitrary tasks (e.g., lambdas, function objects).
- **Thread-safe logging** with log levels (`INFO`, `ERROR`, `RESULT`) for clean output.


## Build Instructions

1. Install CMake (if not already installed)

```bash
# macOS
brew install cmake

# Ubuntu/Debian
sudo apt install cmake
```

2. Clone the repository:

```bash
git clone https://github.com/your-username/taskium.git
cd taskium
```

3. Build with **CMake**:

```bash
mkdir build
cd build
cmake ..
make
```

## Running the Scheduler

An **example program** demonstrates Taskium’s thread pool by running a **Monte Carlo Pi estimation workload** across multiple threads.

Use the **interactive script**:

```bash
./run_monte_carlo.sh
```

The script will:

- Detect your system’s hardware concurrency.
- Prompt for the number of threads and points per thread.
- Submit tasks to the thread pool and run them concurrently.
  
## Example Output (Logging)

```
[INFO] starting pi estimation with 12 threads and 12412 points per thread
[INFO] submitted pi estimation task 0
[INFO] [THREAD 0x16d717000] task 0 started
...
[INFO] [THREAD 0x16d717000] task 11 completed
[RESULT] estimated pi = 3.143007
```

