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
        waitingForCall = true;
        this->callerNumber = callerNumber;
        context.user.showIncomingCall(callerNumber);
        context.timer.startTimer(3000ms);
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

    void ConnectedState::handleDialAction()
    {
        logger.logDebug("ConnectedState: handleDialAction called");
        common::PhoneNumber numberToDial = context.user.getDialedPhoneNumber();
        logger.logDebug("ConnectedState: Dialing number: ", numberToDial);
        context.user.setDialNumber(numberToDial);
        context.user.showDialing();
        context.setState<DialingState>(numberToDial);
    }

}