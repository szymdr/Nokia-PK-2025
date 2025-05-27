#include <iostream>
#include <thread>
#include <atomic>

static std::atomic<int> COUNTER = 0;

void threadFunction()
{
    for( int i = 0; i < 100000; ++i )
    {
        COUNTER.fetch_add(2, std::memory_order_relaxed);
    }
}

int main()
{
    std::thread t1(threadFunction);
    std::thread t2(threadFunction);
    std::thread t3(threadFunction);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Expected value: " << 600000 << std::endl;
    std::cout << "Actual value: " << COUNTER << std::endl;

    return 0;
}