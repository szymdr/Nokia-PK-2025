#include "ConnectedState.hpp"
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

    ConnectedState::~ConnectedState() = default;

void ConnectedState::handleCallRequest(common::PhoneNumber callerNumber)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(3000ms);
    context.setState<ReceivingCallState>(callerNumber);
}

}
