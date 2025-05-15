#pragma once

#include <Messages/PhoneNumber.hpp>
#include <functional>

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleDialAction() = 0;
    virtual void handleCallAccept() = 0;
    virtual void handleCallDrop() = 0;
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
    virtual void showAlert(const std::string &text) = 0;

    virtual common::PhoneNumber getDialedPhoneNumber() const = 0;

    virtual void setDialNumber(const common::PhoneNumber& number) = 0;

    virtual void setAcceptCallback(std::function<void()> callback) = 0;
    virtual void setRejectCallback(std::function<void()> callback) = 0;
};

}
