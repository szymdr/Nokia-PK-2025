#include "WaitingForCallAcceptState.hpp"
#include "ConnectedState.hpp"
#include "TalkingState.hpp"
#include "DialingState.hpp"

namespace ue
{

    WaitingForCallAcceptState::WaitingForCallAcceptState(Context& context, common::PhoneNumber phoneNumber)
        : BaseState(context, "WaitingForCallAcceptState"),
          phoneNumber(phoneNumber)
    {
        //context.user.showDialing();
    }

    WaitingForCallAcceptState::~WaitingForCallAcceptState() = default;

    void WaitingForCallAcceptState::handleCallAccept()
    {
        context.timer.stopTimer();
        context.user.showTalking();
        context.setState<TalkingState>();
    }

    void WaitingForCallAcceptState::handleCallDrop()
    {
        context.timer.stopTimer();
        context.user.showAlert("Call dropped");
        context.setState<ConnectedState>();
    }

    void WaitingForCallAcceptState::handleTimeout()
    {
        context.user.showAlert("Call timeout");
        context.setState<ConnectedState>();
    }

    void WaitingForCallAcceptState::handleDialAction()
    {
        context.setState<DialingState>(phoneNumber);
    }

void WaitingForCallAcceptState::handleUserAcceptCall()
{
    logger.logDebug("WaitingForCallAcceptState: Ignoring handleUserAcceptCall");
        context.user.showTalking();
}

}