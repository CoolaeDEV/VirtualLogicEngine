#include "headers/ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this]() { return stop || !tasks.empty(); });

                    if (stop && tasks.empty()) return;

                    task = std::move(tasks.front());
                    tasks.pop();
                    activeWorkers++;
                }

                task();

                activeWorkers--;
                condition.notify_all();
            }
            });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> job) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(job));
    }
    condition.notify_one();
}

void ThreadPool::waitAll() {
    std::unique_lock<std::mutex> lock(queueMutex);
    condition.wait(lock, [this]() {
        return tasks.empty() && activeWorkers == 0;
    });
}