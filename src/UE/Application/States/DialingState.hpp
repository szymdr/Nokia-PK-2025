#pragma once

#include "BaseState.hpp"

namespace ue
{

class DialingState : public BaseState
{
public:
    DialingState(Context& context, const common::PhoneNumber& phoneNumber);
    ~DialingState() override;

    void handleUserAcceptCall() override;

    void handleTimeout() override;
    void handleDialAction() override;

    void handleCallDrop() override;
    void handleRemoteCallDrop() override;
    void handleUnknownRecipient(common::PhoneNumber phoneNumber) override;

private:
    common::PhoneNumber phoneNumber;
};

}