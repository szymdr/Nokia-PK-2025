#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int main()
{
    std::cout << "Hello, World!" << std::endl;
    
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t([&p]() {
        std::cout << "Hello from thread!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        p.set_value(42);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Thread finished!" << std::endl;
    });

    while (f.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
    {
        std::cout << "Waiting for the thread to finish..." << std::endl;
    }

    std::cout << "Thread finished with value: " << f.get() << std::endl;

    t.join();

    std::cout << "Thread joined!" << std::endl;

    return 0;
}