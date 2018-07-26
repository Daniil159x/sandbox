#include <iostream>
#include "threadpool.hpp"
#include "runnable.hpp"

#include "outwrapper.hpp"

using namespace std::chrono;

class Sender : public Runnable {
public:
    Sender(std::vector<std::string> const& ips) : m_ips(ips) {}
private:
    std::vector<std::string> m_ips;

    // Runnable interface
    virtual void Work_() override {
        for(auto &&ip : m_ips) {
            outTh << "Sending... [ip: " << ip << "]";
        }
        outTh << "All sended";
    }
};

void LongWork() {
    outTh << "lond work[1]" ;
    std::this_thread::sleep_for(seconds(2));
    outTh << "lond work[2]" ;
    std::this_thread::sleep_for(seconds(2));
    outTh << "lond work[3]" ;
    std::this_thread::sleep_for(seconds(2));
    outTh << "lond work[4]" ;
    std::this_thread::sleep_for(seconds(2));
    outTh << "lond work is done" ;
}

int Summing(std::vector<int> const &vec) {
    int res = 0;
    for(auto&& i : vec) {
        res += i;
    }
    return res;
}

int Exception() {
    outTh << __PRETTY_FUNCTION__ << " sleeping on 2 sec" ;
    throw std::logic_error("XAXAXA");
}

int main()
{
    ThreadPool pool;

    // 1
    Sender s = std::vector<std::string>{
        "192.168.1.1", "127.0.0.1", "10.10.10.10"
    };
    s.Run(pool);

    // 2
    pool.AddTask(&LongWork);

    // 3
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto fut = pool.AddAnyTask(&Summing, vec);

    // 4
    pool.AddAnyTaskRef([](std::future<int> &f){
        int count = f.get();
        while(count--){
            outTh << "Notify" ;
            std::this_thread::sleep_for(milliseconds(100));
        }
    }, fut);

    // 5
    const std::string toHash = "Hello world!!!!";
    auto hash_fur = pool.AddAnyTask(std::hash<std::string>(), toHash);
    auto calc_hash = hash_fur.get();
    outTh << "hash(" << toHash << ") = " << std::hex << calc_hash ;

    // 6
    try {
        pool.AddAnyTask(&Exception).get();
    } catch (std::exception const& e) {
        outTh << "Exception: " << e.what() ;
    }

    pool.Join();
    return 0;
}

