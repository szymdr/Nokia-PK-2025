// c++
#include "TimerPort.hpp"
#include <chrono>
#include <thread>

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

TimerPort::~TimerPort()
{
    stopTimer();
    if (timerThread.joinable())
    {
        timerThread.join();
    }
}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stopped");
    stopTimer();
    handler = nullptr;
}

void TimerPort::startTimer(Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    stopTimer();

    std::thread(&TimerPort::startCountdown, this, duration).detach();
}

void TimerPort::startCountdown(Duration duration)
{
    logger.logDebug("Start countdown timer: ", duration.count(), "ms");
    stopTimer();
    timerRunning.store(true);
    timerThread = std::thread([this, duration]() {
        auto startTime = std::chrono::steady_clock::now();
        while (timerRunning.load() && std::chrono::steady_clock::now() - startTime < duration)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (timerRunning.load() && handler)
        {
            handler->handleTimeout();
        }
        timerRunning.store(false);
    });
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");
    timerRunning.store(false);
    if (timerThread.joinable())
    {
        timerThread.join();
    }
}
}