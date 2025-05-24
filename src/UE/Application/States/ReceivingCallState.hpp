#pragma once

#include "BaseState.hpp"

namespace ue
{

class ReceivingCallState : public BaseState
{
public:
    ReceivingCallState(Context &context, const common::PhoneNumber& callerNumber);
    ~ReceivingCallState() override;

    void handleUserAcceptCall() override;
    void handleCallDrop() override;
    void handleRemoteCallDrop() override;
    void handleTimeout() override;

private:
    common::PhoneNumber callerNumber;
};
}