#include <iostream>
#include <thread>
#include <mutex>

static int COUNTER = 0;

void threadFunction(std::mutex& mtx, std::mutex& mtx2)
{
    for( int i = 0; i < 1000; ++i )
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::lock_guard<std::mutex> lock2(mtx2);

        std::cout << std::this_thread::get_id() << "-" <<COUNTER++ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
}

int main()
{
    std::mutex mtx;
    std::mutex mtx2;
    std::thread t1(threadFunction, std::ref(mtx), std::ref(mtx2));
    std::thread t2(threadFunction, std::ref(mtx), std::ref(mtx2));
    std::thread t3(threadFunction, std::ref(mtx), std::ref(mtx2));

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Expected value: " << 3000 << std::endl;
    std::cout << "Actual value: " << COUNTER << std::endl;

    return 0;
}