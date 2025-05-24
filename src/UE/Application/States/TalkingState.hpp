#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context, const common::PhoneNumber& talkingToPhoneNumber);
    ~TalkingState() override;

    void handleCallDrop() override;
    void handleRemoteCallDrop() override;
    void handleUnknownRecipient(common::PhoneNumber) override;
    void handleCallRequest(common::PhoneNumber) override;

private:
    common::PhoneNumber talkingToPhoneNumber;
};

}
