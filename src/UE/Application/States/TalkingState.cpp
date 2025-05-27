#include "TalkingState.hpp"

#include "ConnectedState.hpp"
#include "Context.hpp"
#include "NotConnectedState.hpp"

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

void TalkingState::handleRemoteCallDrop(common::PhoneNumber phoneNumber)
{
    if (phoneNumber == talkingToPhoneNumber) {
        context.timer.stopTimer();
        context.setState<ConnectedState>();
        context.user.showAlert("Call ended");
    }
}

void TalkingState::handleUnknownRecipient(common::PhoneNumber /*phoneNumber*/)
{
    context.setState<ConnectedState>();
    context.user.showAlert("Peer unavailable");
}

void TalkingState::handleCallRequest(common::PhoneNumber phoneNumber)
{
    context.bts.sendCallDrop(phoneNumber);
}

void TalkingState::handleTimeout()
{
    context.bts.sendCallDrop(talkingToPhoneNumber);
    context.setState<ConnectedState>();
    context.user.showAlert("Call ended due to inactivity");

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

void TalkingState::resetInactivityTimer() const
{
    context.timer.startTimer(std::chrono::minutes(2));
}

void TalkingState::handleSmsReceived(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber)
{
    SmsDb &db = context.user.getSmsDb();
    db.addSms(text, fromPhoneNumber, toPhoneNumber);
}

void TalkingState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

}
