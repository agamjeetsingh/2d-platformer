//
// Created by Agamjeet Singh on 14/09/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

class ThreadPool {
public:
    explicit ThreadPool(size_t thread_count = DEFAULT_THREAD_COUNT, size_t capacity = DEFAULT_CAPACITY);

    template <typename Func, typename... Args>
    auto enqueue(Func&& f, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>> {
        using R = std::invoke_result_t<Func, Args...>;
        auto task = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );

        std::future<R> future = task->get_future();

        {
            std::unique_lock lock{queue_mtx};
            queue_not_full.wait(lock, [this](){ return stop || queue.size() < capacity; });
            if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            queue.emplace([task]() { (*task)(); });
        }
        queue_not_empty.notify_one();
        return future;
    }

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    static constexpr size_t DEFAULT_CAPACITY = 8;
    static constexpr size_t DEFAULT_THREAD_COUNT = 4;
    const size_t capacity = 0; // Capacity of threads that can be waiting
    std::mutex queue_mtx;
    std::condition_variable queue_not_empty, queue_not_full;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> queue;
    std::atomic<bool> stop{false};
};



#endif //THREADPOOL_H
