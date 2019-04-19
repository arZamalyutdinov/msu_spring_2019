#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

constexpr uint64_t maxTimes = 1000000;

void ping(std::mutex &mtx, std::condition_variable &cond, uint64_t &count)
{
    for (; count < maxTimes;) {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [&count]{
            return count % 2 == 0;
        });
        std::cout << "ping\n";
        ++count;
        cond.notify_one();
        std::this_thread::yield();
    }
}

void pong(std::mutex &mtx, std::condition_variable &cond, uint64_t &count)
{
    for (; count < maxTimes;) {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [&count]{
            return count % 2;
        });
        std::cout << "pong\n";
        ++count;
        cond.notify_one();
        std::this_thread::yield();
    }
}

int main(int argc, char const *argv[])
{
    std::mutex mtx;
    std::condition_variable cond;
    uint64_t count = 0;
    std::thread thr1(ping, std::ref(mtx), std::ref(cond), std::ref(count));
    std::thread thr2(pong, std::ref(mtx), std::ref(cond), std::ref(count));
    thr1.join();
    thr2.join();
    return 0;
}