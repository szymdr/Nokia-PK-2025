#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

enum class State
{
    TICK,
    TACK,
    TOE
};

static State nextStep = State::TICK;

void tick(std::mutex& mtx, std::condition_variable& cv)
{
    for ( int i = 0; i < 10; ++i )
    {
        // while ( nextStep != State::TICK )
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return nextStep == State::TICK; });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Tick" << std::endl;
        nextStep = State::TACK;
        cv.notify_all(); 
    }
}

void tack(std::mutex& mtx, std::condition_variable& cv)
{
    for ( int i = 0; i < 10; ++i )
    {
        // while ( nextStep != State::TACK )
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return nextStep == State::TACK; });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Tack" << std::endl;
        nextStep = State::TOE;
        cv.notify_all(); 
    }
}

void toe(std::mutex& mtx, std::condition_variable& cv)
{
    for ( int i = 0; i < 10; ++i )
    {
        // while ( nextStep != State::TOE )
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return nextStep == State::TOE; });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Toe" << std::endl;
        nextStep = State::TICK;
        cv.notify_all(); 
    }
}

int main()
{
    std::mutex mtx;
    std::condition_variable cv;
    std::thread t1(tick, std::ref(mtx), std::ref(cv));
    std::thread t2(tack, std::ref(mtx), std::ref(cv));
    std::thread t3(toe, std::ref(mtx), std::ref(cv));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}