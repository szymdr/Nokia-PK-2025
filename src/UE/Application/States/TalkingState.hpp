#pragma once

#include "BaseState.hpp"

namespace ue
{

class TalkingState : public BaseState
{
public:
    TalkingState(Context& context);
    ~TalkingState() override;
    
    void handleUnknownRecipient(common::PhoneNumber phoneNumber) override;
    void handleCallRequest(common::PhoneNumber phoneNumber) override;
};

}
