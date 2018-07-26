#include "threadpool.hpp"

ThreadPool::ThreadPool(const size_t nThreadCount) : m_workers(nThreadCount) {

    for(auto &t : m_workers) {
        t = std::thread(&ThreadPool::WorkersLoop_, this);
    }
}

void ThreadPool::AddTask(ThreadPool::task_t foo) {

    std::unique_lock<std::mutex> lock(m_mtxQueue);
    m_taskQueue.push(std::move(foo));
    lock.unlock();
    m_cvAddedTask.notify_one();

}

size_t ThreadPool::GetCountTask() noexcept
{
    std::lock_guard<std::mutex> lock(m_mtxQueue);
    return m_taskQueue.size();
}

ThreadPool::~ThreadPool() noexcept {
    std::unique_lock<std::mutex> lock(m_mtxQueue);
    m_exit = true;
    lock.unlock();

    m_cvAddedTask.notify_all();
    for(auto &t : m_workers) {
        if(t.joinable()) {
            t.join();
        }
    }
}

void ThreadPool::Join() noexcept
{
    std::unique_lock<std::mutex> lock(m_mtxQueue);
    m_exit = true;
    lock.unlock();

    m_cvAddedTask.notify_all();
    for(auto &t : m_workers) {
        if(t.joinable()) {
            t.join();
        }
    }
}

void ThreadPool::WorkersLoop_() noexcept {

    std::unique_lock<std::mutex> lock(m_mtxQueue);

    while (!m_exit && !m_exitIfEmpty) {
        m_cvAddedTask.wait(lock, [&](){ return !m_taskQueue.empty() || m_exit || m_exitIfEmpty; });
        if(m_exit) return;

        while(!m_taskQueue.empty()) {
            task_t localTask = std::move(m_taskQueue.front());
            m_taskQueue.pop();
            lock.unlock();

            try {
                localTask();
            } catch (...) { /* TODO */ }

            lock.lock();
        }
    }
}