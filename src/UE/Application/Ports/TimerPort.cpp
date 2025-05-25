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
    TimerPort::stopTimer();
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
    std::thread(&TimerPort::runTimer, this, duration).detach();
}

void TimerPort::stopTimer()
{
    logger.logDebug("Stop timer");
    isTimerRunning = false;
}

void TimerPort::runTimer(Duration duration)
{
    std::this_thread::sleep_for(duration);
    if (isTimerRunning)
    {
        logger.logDebug("Timer expired");
        handler->handleTimeout();
    }
    else
    {
        logger.logDebug("Timer stopped before expiration");
    }
}

}