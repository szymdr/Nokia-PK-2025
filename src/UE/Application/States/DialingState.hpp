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
    void handleRemoteCallDrop(common::PhoneNumber phoneNumber) override;
    void handleUnknownRecipient(common::PhoneNumber phoneNumber) override;
    void handleSmsReceived(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    void handleDisconnected() override;
    void handleCallRequest(common::PhoneNumber) override;

private:
    common::PhoneNumber phoneNumber;
};

}