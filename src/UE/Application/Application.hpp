#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;

    void handleCallRequest(PhoneNumber phoneNumber) override;
    void handleUserAcceptCall() override;
    void handleUserRejectCall() override;
    void handleUnknownRecipient(PhoneNumber phoneNumber) override;

    void handleDialAction() override;
    void handleCallAccept() override;
    void handleCallDrop() override;
    void handleSmsReceived(const std::string& text,common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    void handleSmsCompose() override;
    void handleViewSmsList() override;
    void handleViewSmsAccept() override;
    void handleViewSmsClose() override;
    void handleSmsSend() override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
