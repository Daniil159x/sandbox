#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <future>

#include "utils.hpp"

#include "../function/function.hpp"

class ThreadPool {
public:
    using task_t = MyFoo<void()>;

    /*explicit*/ ThreadPool(const size_t nThreadCount = std::thread::hardware_concurrency());

    void AddTask(task_t foo);

    // for called types { f(...) }
    template <typename Some, typename ...Args>
    auto AddAnyTask(Some &&f, Args&& ...args) -> std::future< decltype( f(std::forward<Args>(args)...) ) >
    {
        using RetT = decltype( f(std::forward<Args>(args)...) );

        auto l = detali::CopyLambdaCpp14<Some, RetT(Args...)> {std::forward<Some>(f),
                                                               std::forward<Args>(args)...};
        auto future = l.GetFuture();
        AddTask( std::move(l) );

        return future;
    }

    // for called types { f(...) }
    // NOTE: DANGEROUS, not thread-safe task! but no copies
    template <typename Some, typename ...Args>
    auto AddAnyTaskRef(Some &&f, Args&& ...args) -> std::future< decltype( f(std::forward<Args>(args)...) ) >
    {
        using RetT = decltype( f(std::forward<Args>(args)...) );

        auto l = detali::RefLambdaCpp14<Some, RetT(Args...)> {std::forward<Some>(f),
                                                              std::forward<Args>(args)...};
        auto future = l.GetFuture();
        AddTask( std::move(l) );

        return future;
    }


    // есть ли вообще смысл в этих функциях? (по идеи, эти функции это разширение предыдущей на любой метод)

    // for member types c->f(...)
//    template <typename SomeF, typename C, typename ...Args>
//    auto AddAnyTask(SomeF &&f, C &&c, Args&& ...args) -> std::future< decltype( (c->*f)(std::forward<Args>(args)...) ) >
//    {}
            // TODO: нужно копировать весь объект к себе

    // for member types c->f(...)
//    template <typename SomeF, typename C, typename ...Args>
//    auto AddAnyTask(SomeF &&f, C &&c, Args&& ...args) -> std::future< decltype( (c.*f)(std::forward<Args>(args)...) ) >
//    {}
            // TODO: нужно копировать весь объект к себе

    size_t GetCountTask() noexcept;

    ~ThreadPool() noexcept;

    void Join() noexcept;

private:
    std::mutex               m_mtxQueue;
    std::queue<task_t>       m_taskQueue;
    std::vector<std::thread> m_workers;

    std::condition_variable  m_cvAddedTask;
    bool                     m_exit = false;
    bool                     m_exitIfEmpty = false;


    void WorkersLoop_() noexcept;
};

#endif // THREADPOOL_HPP