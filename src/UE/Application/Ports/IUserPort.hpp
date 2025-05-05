#pragma once

#include <Messages/PhoneNumber.hpp>

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showIncomingCall(common::PhoneNumber) = 0;
    virtual void showDialing() = 0;
    virtual void showTalking() = 0;
};

}
