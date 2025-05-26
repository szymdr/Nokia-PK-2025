#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    TimerPort(common::ILogger& logger);
    ~TimerPort() override;

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;

    std::atomic<bool> isTimerRunning{false};
    std::thread timerThread;
    std::condition_variable timerCv;
    std::mutex timerMutex;
    bool stopRequested = false;

    void runTimer(Duration duration);
};

}
