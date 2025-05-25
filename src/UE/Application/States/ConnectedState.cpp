#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
#include "ReceivingCallState.hpp"
#include "DialingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    logger.logDebug("ConnectedState: Entered");
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

ConnectedState::~ConnectedState() = default;

void ConnectedState::handleCallRequest(common::PhoneNumber callerNumber)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(30000ms);
    context.setState<ReceivingCallState>(callerNumber);
}

void ConnectedState::handleDialAction()
{
    logger.logDebug("ConnectedState: handleDialAction called");
    common::PhoneNumber numberToDial = context.user.getDialedPhoneNumber();

    logger.logDebug("ConnectedState: Dialing number: ", numberToDial);
    context.user.setDialNumber(numberToDial);

    context.bts.sendCallRequest(numberToDial);
    context.timer.startTimer(std::chrono::seconds(60));
    context.setState<DialingState>(numberToDial);
}

void ConnectedState::handleCallDrop()
{
}

void ConnectedState::handleUnknownRecipient(common::PhoneNumber)
{
}

}