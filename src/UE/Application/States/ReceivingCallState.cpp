#include "ReceivingCallState.hpp"
#include "ConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ReceivingCallState::ReceivingCallState(Context &context, const common::PhoneNumber& callerNumber)
    : BaseState(context, "ReceivingCallState"), callerNumber(callerNumber)
{
    logger.logDebug("ReceivingCallState: Entered");
    context.user.showIncomingCall(callerNumber);
}

ReceivingCallState::~ReceivingCallState()
{
    logger.logDebug("ReceivingCallState: Exiting");
}

void ReceivingCallState::handleUserAcceptCall()
{
    context.timer.stopTimer();
    context.bts.sendCallAccept(callerNumber);
    context.setState<TalkingState>();
}

void ReceivingCallState::handleUserRejectCall()
{
    context.timer.stopTimer();
    context.bts.sendCallDrop(callerNumber);
    context.setState<ConnectedState>();
}

void ReceivingCallState::handleTimeout()
{
    context.bts.sendCallDrop(callerNumber);
    context.setState<ConnectedState>();
}

}