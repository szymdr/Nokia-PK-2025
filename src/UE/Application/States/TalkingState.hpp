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
    void handleTimeout() override;
    void handleUserCallTalk(const std::string& text) override;
    void handleBtsCallTalk(const std::string& text) override;

private:
    void sendMessage(const std::string& text);
    void resetInactivityTimer() const;

    common::PhoneNumber talkingToPhoneNumber;
    bool lastActionWasSending{false};
};

}
