#pragma once

#include <Messages/PhoneNumber.hpp>
#include <functional>
#include "IUeGui.hpp"
#include <SmsDB.hpp>
namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;

    virtual void handleDialAction() = 0;
    virtual void handleCallAccept() = 0;
    virtual void handleCallDrop() = 0;
    virtual void handleViewSmsAccept() = 0;
    virtual void handleViewSmsClose() = 0;
    virtual void handleViewSmsList() = 0;
    virtual void handleSmsCompose() = 0;
    virtual void handleSmsSend() = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showIncomingCall(const common::PhoneNumber) = 0;
    virtual void showDialing() = 0;
    virtual void showTalking() = 0;
    virtual void showAlert(const std::string &text) = 0;
    virtual common::PhoneNumber getDialedPhoneNumber() const = 0;
    virtual void setDialNumber(const common::PhoneNumber& number) = 0;
    virtual IUeGui::ISmsComposeMode& getSmsComposeMode() = 0;
    virtual void showSmsList() = 0;
    virtual void showSms(int index) = 0;
    virtual int getAction() = 0;
    virtual SmsDb& getSmsDb() = 0;
    virtual void setAcceptCallback(IUeGui::Callback) = 0;
    virtual void setRejectCallback(IUeGui::Callback) = 0;
    virtual void showSmsComposerView() = 0;
};

}
