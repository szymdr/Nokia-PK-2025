#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"
#include "ReceivingCallState.hpp"
#include "ViewingSmsListState.hpp"
#include "SendingSmsState.hpp"
#include "DialingState.hpp"
#include <UeGui/IListViewMode.hpp>


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

void ConnectedState::handleSmsReceived(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    SmsDb &db = context.user.getSmsDb();
    db.addSms(text, fromPhoneNumber, toPhoneNumber);
    context.user.showAlert("NEW MESSAGE!");
}
void ConnectedState::handleSmsCompose()
{
    logger.logDebug("ConnectedState: Composing SMS");
    context.setState<SendingSmsState>();
}
void ConnectedState::handleViewSmsList()
{
    logger.logDebug("ConnectedState: Viewing SMS list");
    context.setState<ViewingSmsListState>();
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