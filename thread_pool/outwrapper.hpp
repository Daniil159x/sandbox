#ifndef OUTWRAPPER_HPP
#define OUTWRAPPER_HPP

#include <mutex>
#include <iostream>

class OutWrapper{
public:
    std::mutex m_mtx;
} static outTh;

class OutWrapperContinue {
public:
    OutWrapperContinue(std::mutex& mtx) : m_lock(mtx) {}
    OutWrapperContinue(OutWrapperContinue&&) = default;

    ~OutWrapperContinue(){
        std::cout << std::endl;
    }

    std::unique_lock<std::mutex> m_lock;
};

template <class T>
OutWrapperContinue&& operator << (OutWrapperContinue&& out, T&& val) {

    std::cout << val;

    return std::move(out);
}

template <class T>
OutWrapperContinue operator << (OutWrapper& out, T&& val) {
    OutWrapperContinue cont(out.m_mtx);

    std::cout << val;

    return cont;
}


#endif // OUTWRAPPER_HPP