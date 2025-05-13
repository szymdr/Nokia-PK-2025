#include "DialingState.hpp"
#include "ConnectedState.hpp"
#include "TalkingState.hpp"
#include "WaitingForCallAcceptState.hpp"

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
        context.bts.sendCallRequest(phoneNumber);
        context.timer.startTimer(std::chrono::seconds(60));
        context.setState<WaitingForCallAcceptState>(phoneNumber);
    }

    void DialingState::handleUserRejectCall()
    {
        context.setState<ConnectedState>();
    }

    void DialingState::onEnter()
{
    logger.logDebug("DialingState: Entered");
    context.bts.sendCallRequest(phoneNumber);
    context.timer.startTimer(std::chrono::seconds(60));
    context.user.showDialing();
}

void DialingState::handleTimeout()
{
    context.user.showAlert("Call timeout");
    context.setState<ConnectedState>();
}

void DialingState::handleDialAction()
{
        logger.logDebug("DialingState: handleDialAction triggered");
        context.bts.sendCallRequest(phoneNumber);
        context.timer.startTimer(std::chrono::seconds(60));
        context.setState<WaitingForCallAcceptState>(phoneNumber);
}

void DialingState::handleCallAccept()
{
    logger.logDebug("DialingState: Call accepted");
    context.user.showTalking();
    context.setState<TalkingState>();
}

}