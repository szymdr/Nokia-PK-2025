#pragma once

#include <gmock/gmock.h>
#include <Messages/PhoneNumber.hpp>

#include "Ports/IUserPort.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showIncomingCall, (common::PhoneNumber), (final));
    MOCK_METHOD(void, showDialing, (), (final));
    MOCK_METHOD(void, showTalking, (), (final));
};

}
