#include "TalkingState.hpp"

#include "ConnectedState.hpp"
#include "Context.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, const common::PhoneNumber &talkingToPhoneNumber)
  : BaseState(context, "TalkingState"), talkingToPhoneNumber(talkingToPhoneNumber)
{
    context.user.showTalking();
    resetInactivityTimer();
}

TalkingState::~TalkingState() = default;

void TalkingState::handleCallDrop()
{
    context.bts.sendCallDrop(talkingToPhoneNumber);
    context.setState<ConnectedState>();
}

void TalkingState::handleRemoteCallDrop()
{
    context.user.showAlert("Call ended");
    context.setState<ConnectedState>();
}

void TalkingState::handleUnknownRecipient(common::PhoneNumber /*phoneNumber*/)
{
    context.user.showAlert("Peer unavailable");
    context.setState<ConnectedState>();
}

void TalkingState::handleCallRequest(common::PhoneNumber)
{
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(talkingToPhoneNumber);
    context.user.showAlert("Call ended due to inactivity");
    context.setState<ConnectedState>();
}

void TalkingState::handleBtsCallTalk(const std::string& text)
{
    context.user.appendIncomingText("Peer: " + text);
    lastActionWasSending = false;
    resetInactivityTimer();
}

void TalkingState::handleUserCallTalk(const std::string& text)
{
    context.bts.sendCallTalk(talkingToPhoneNumber, text);
    context.user.appendIncomingText("You: " + text);
    lastActionWasSending = true;
    resetInactivityTimer();
}

void TalkingState::sendMessage(const std::string& text)
{
    context.bts.sendCallTalk(talkingToPhoneNumber, text);
    context.user.appendIncomingText("You: " + text);
    lastActionWasSending = true;
    resetInactivityTimer();
}

void TalkingState::resetInactivityTimer()
{
    context.timer.startTimer(std::chrono::minutes(2));
}

}
