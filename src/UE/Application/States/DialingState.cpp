#include "DialingState.hpp"
#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
namespace ue
{

DialingState::DialingState(Context& context, const common::PhoneNumber& phoneNumber)
    : BaseState(context, "DialingState"), phoneNumber(phoneNumber)
{
    logger.logDebug("DialingState: Created with phone number: ", phoneNumber);
    context.user.showCalling(phoneNumber);

}

DialingState::~DialingState() = default;

void DialingState::handleUserAcceptCall()
{
    context.timer.stopTimer();
    context.setState<TalkingState>(phoneNumber);
}

void DialingState::handleTimeout()
{
    context.setState<ConnectedState>();
    context.user.showAlert("Call timeout");
}

void DialingState::handleDialAction()
{

}

void DialingState::handleRemoteCallDrop(common::PhoneNumber phoneNumber)
{
    if (phoneNumber == this->phoneNumber) {
        context.timer.stopTimer();
        context.setState<ConnectedState>();
        context.user.showAlert("Call not accepted");
    }
}

void DialingState::handleUnknownRecipient(common::PhoneNumber phoneNumber)
{
    context.timer.stopTimer();
    context.setState<ConnectedState>();
    context.user.showAlert("Peer UE not connected");
}

void DialingState::handleCallDrop()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(phoneNumber);
    context.setState<ConnectedState>();
}

void DialingState::handleSmsReceived(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber)
{
    SmsDb &db = context.user.getSmsDb();
    db.addSms(text, fromPhoneNumber, toPhoneNumber);
}

void DialingState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void DialingState::handleCallRequest(common::PhoneNumber phoneNumber)
{
    context.bts.sendCallDrop(phoneNumber);
}

}