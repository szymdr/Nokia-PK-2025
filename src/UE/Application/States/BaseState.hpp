#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleDisconnected() override;

    // IUserEventsHandler interface
    void handleCallRequest(common::PhoneNumber phoneNumber) override;
    void handleUserAcceptCall() override;
    void handleUserRejectCall() override;
    void handleUnknownRecipient(common::PhoneNumber phoneNumber) override;

    void handleDialAction() override;
    void handleCallAccept() override;
    void handleCallDrop() override;
    void handleSmsReceived(const std::string &text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    void handleSmsCompose() override;
    void handleViewSmsList() override;
    void handleViewSmsAccept() override;
    void handleViewSmsClose() override;
    void handleSmsSend() override;
    void onEnter() {}

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
