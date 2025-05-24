#include "DialingState.hpp"
#include "ConnectedState.hpp"
#include "TalkingState.hpp"
namespace ue
{

DialingState::DialingState(Context& context, const common::PhoneNumber& phoneNumber)
    : BaseState(context, "DialingState"), phoneNumber(phoneNumber)
{
    logger.logDebug("DialingState: Created with phone number: ", phoneNumber);

}

DialingState::~DialingState() = default;

void DialingState::handleUserAcceptCall()
{
    context.timer.stopTimer();
    context.bts.sendCallRequest(phoneNumber); // Wyślij żądanie połączenia
    context.timer.startTimer(std::chrono::seconds(60)); // Uruchom timer
    context.setState<TalkingState>();
}

void DialingState::handleUserRejectCall()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(phoneNumber);
    context.user.showAlert("Call dropped");
    context.setState<ConnectedState>();
}

void DialingState::onEnter()
{
    this->handleDialAction();
}

void DialingState::handleTimeout()
{
    context.user.showAlert("Call timeout");
    context.setState<ConnectedState>();
}

void DialingState::handleDialAction()
{
    auto toNumber = context.user.getDialedPhoneNumber();
    logger.logDebug("DialingState: handleDialAction, dialing: ", toNumber);

    context.bts.sendCallRequest(toNumber);
    context.timer.startTimer(std::chrono::seconds(60));
}

void DialingState::handleCallAccept()
{
     context.timer.stopTimer();
     context.setState<TalkingState>();
}

void DialingState::handleCallDrop()
{
    context.timer.stopTimer();
    context.user.showAlert("Call dropped");
    context.setState<ConnectedState>();
}

void DialingState::handleUnknownRecipient(common::PhoneNumber phoneNumber)
{
    context.timer.stopTimer();
    context.user.showAlert("Peer UE not connected");
    context.setState<ConnectedState>();
}

}