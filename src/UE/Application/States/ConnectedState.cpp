#include "ConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

    ConnectedState::~ConnectedState() = default;

void ConnectedState::handleCallRequest(common::PhoneNumber number)
{
    using namespace std::chrono_literals;
    waitingForCall = true;
    callerNumber    = number;
    context.user.showIncomingCall(callerNumber);
    context.timer.startTimer(3000ms);
    context.bts.sendCallRequest(callerNumber);
}

void ConnectedState::handleUserAcceptCall()
{
    if (!waitingForCall) return;
    waitingForCall = false;
    context.timer.stopTimer();
    context.user.showTalking();
    context.bts.sendCallAccept(callerNumber);
    context.setState<TalkingState>();
}

void ConnectedState::handleUserRejectCall()
{
    if (!waitingForCall) return;
    waitingForCall = false;
    context.timer.stopTimer();
    context.user.showConnected();
    context.bts.sendCallDrop(callerNumber);
}

void ConnectedState::handleTimeout()
{
    if (!waitingForCall) return;
    waitingForCall = false;
    context.user.showConnected();
    context.bts.sendCallDrop(callerNumber);
}

}
