#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <thread>
#include <atomic>

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
    void startCountdown(Duration duration) override;
    void stopTimer() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;
    std::thread timerThread;
    std::atomic<bool> timerRunning{false};
};

}
