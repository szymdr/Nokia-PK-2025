#pragma once

#include <gmock/gmock.h>
#include "Ports/ITimerPort.hpp"

namespace ue
{

class ITimerEventsHandlerMock : public ITimerEventsHandler
{
public:
    ITimerEventsHandlerMock();
    ~ITimerEventsHandlerMock() override;

    MOCK_METHOD(void, handleTimeout, (), (final));

};

class ITimerPortMock : public ITimerPort
{
public:
    ITimerPortMock();
    ~ITimerPortMock() override;

    MOCK_METHOD(void, startTimer, (ITimerPort::Duration), (final));
    MOCK_METHOD(void, startCountdown, (ITimerPort::Duration), (final));
    MOCK_METHOD(void, stopTimer, (), (final));
};

}
