#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
#include "ReceivingCallState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    logger.logDebug("ConnectedState: Entered");
    context.user.showConnected();
}
void ConnectedState::handleDisconnected(){
    context.setState<NotConnectedState>();
}
    ConnectedState::~ConnectedState() = default;

void ConnectedState::handleCallRequest(common::PhoneNumber callerNumber)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(3000ms);
    context.setState<ReceivingCallState>(callerNumber);
}

}
