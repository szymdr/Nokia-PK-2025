// c++
#include "TimerPort.hpp"
#include <chrono>

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

TimerPort::~TimerPort()
{
    stopTimer();
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

    isTimerRunning = true;
    stopRequested = false;

    timerThread = std::thread(&TimerPort::runTimer, this, duration);
}

void TimerPort::stopTimer()
{
    {
        std::lock_guard<std::mutex> lock(timerMutex);
        stopRequested = true;
    }
    timerCv.notify_all();

    if (timerThread.joinable())
    {
        timerThread.join();
    }

    isTimerRunning = false;
    logger.logDebug("Timer stopped");
}

void TimerPort::runTimer(Duration duration)
{
    std::unique_lock<std::mutex> lock(timerMutex);
    if (timerCv.wait_for(lock, duration, [this] { return stopRequested; }))
    {
        logger.logDebug("Timer stopped before expiration");
        return;
    }

    if (isTimerRunning && handler)
    {
        logger.logDebug("Timer expired");
        handler->handleTimeout();
    }
}

}