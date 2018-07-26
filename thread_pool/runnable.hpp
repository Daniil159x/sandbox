#ifndef RUNNABLE_HPP
#define RUNNABLE_HPP

#include "threadpool.hpp"

// Object must live while Work_() is being executed
class Runnable {
public:

    void Run(ThreadPool &pool) {
        pool.AddTask([&](){
            Work_();
        });
    }

    virtual ~Runnable() = default;

protected:

    virtual void Work_() = 0;
};


#endif // RUNNABLE_HPP
