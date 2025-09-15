//
// Created by Agamjeet Singh on 14/09/25.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t thread_count, size_t capacity): capacity(capacity) {
    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back([this]() {
            while (true) {
                std::function<void()> func;
                {
                    std::unique_lock lock{queue_mtx};
                    queue_not_empty.wait(lock, [this]() {
                        return stop || !queue.empty();
                    });
                    if (stop && queue.empty()) return;
                    func = std::move(queue.front());
                    queue.pop();
                    queue_not_full.notify_one();
                }
                try {
                    func();
                } catch (...) {
                    // Do nothing
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock{queue_mtx};
        stop = true;
    }
    queue_not_empty.notify_all();
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}
