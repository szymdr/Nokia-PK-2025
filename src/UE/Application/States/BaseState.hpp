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

    // IUserEventsHandler interface
    void handleCallRequest(common::PhoneNumber phoneNumber) override;
    void handleUserAcceptCall() override;
    void handleUserRejectCall() override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
