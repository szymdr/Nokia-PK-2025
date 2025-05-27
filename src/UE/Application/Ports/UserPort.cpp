#include "UserPort.hpp"

#include <UeGui/ICallMode.hpp>
#include <UeGui/IDialMode.hpp>
#include <UeGui/ITextMode.hpp>
#include <UeGui/ISmsComposeMode.hpp>
#include <UeGui/IListViewMode.hpp>
#include <Sms.hpp>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      callerNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(callerNumber));
}

void UserPort::stop()
{
    handler = nullptr;
}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Dial number", "");
    gui.setAcceptCallback([this]() {
        auto selection = gui.setListViewMode().getCurrentItemIndex();
        if (selection.first)
        {
            handleMenuSelection(selection.second);
        }
    });
}

void UserPort::showIncomingCall(const common::PhoneNumber callerNumber)
{
    auto& callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.appendIncomingText("Incoming call from: " + to_string(callerNumber));

    gui.setAcceptCallback([this]() {
        if (handler)
        {
            handler->handleCallAccept();
        }
    });

    gui.setRejectCallback([this]() {
        if (handler)
        {
            handler->handleCallDrop();
        }
    });
}

void UserPort::showDialing()
{
    auto& dialMode = gui.setDialMode();

    gui.setAcceptCallback([this, &dialMode]() {
        if (handler)
        {
            dialedPhoneNumber = dialMode.getPhoneNumber();
            logger.logDebug("UserPort: Dial number set to: ", dialedPhoneNumber);

            auto& callMode = gui.setCallMode();
            callMode.clearIncomingText();
            callMode.appendIncomingText("Dialing to: " + to_string(dialedPhoneNumber));

            handler->handleDialAction();
        }
    });

    gui.setRejectCallback([this]() {
        if (handler)
        {
            handler->handleCallDrop();
        }
    });

}

void UserPort::showTalking()
{
    gui.setCallMode().clearIncomingText();
    gui.setCallMode().appendIncomingText("Talking...");
}

void UserPort::showAlert(const std::string &text)
{
    gui.setAlertMode().setText("" + text);

}

common::PhoneNumber UserPort::getDialedPhoneNumber() const
{
    return dialedPhoneNumber;
}

void UserPort::setDialNumber(const common::PhoneNumber& number)
{
    //dialedPhoneNumber = number;
    gui.setDialMode();
    logger.logDebug("UserPort: Dial number set to: ", dialedPhoneNumber);

}


void UserPort::showSmsList() {
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    for (auto&& sms : smsDb.getSmsList()) {
        std::string info = "FROM: " + common::to_string(sms.getFrom()) + "TO: " + common::to_string(sms.getTo());
        menu.addSelectionListItem(info, "");
    }

    gui.setAcceptCallback([this, &menu] { onAcceptSmsList(menu); });
    gui.setRejectCallback([this] {
        if (handler) handler->handleViewSmsClose();
    });
}

void UserPort::onAcceptSmsList(IUeGui::IListViewMode& menu) {
    auto selection = menu.getCurrentItemIndex();
    if (selection.first) {
        action = selection.second;
        if (handler) handler->handleViewSmsAccept();
    }
}

void UserPort::showSms(int index) {
    auto smsOpt = smsDb.retrieveSms(index);
    if (smsOpt) {
        gui.setViewTextMode().setText(smsOpt->getText());
    } else {
        gui.setAlertMode().setText("SMS not found");
    }

    gui.setAcceptCallback([this] {
        if (handler) handler->handleViewSmsAccept();
    });
    gui.setRejectCallback([this] {
        if (handler) handler->handleViewSmsClose();
    });
}

int UserPort::getAction() {
    return action;
}

SmsDb& UserPort::getSmsDb() {
    return smsDb;
}
IUeGui::ISmsComposeMode& UserPort::getSmsComposeMode()
{
    return gui.setSmsComposeMode();
}
void UserPort::setAcceptCallback(IUeGui::Callback callback)
{
    gui.setAcceptCallback(callback);
}

void UserPort::setRejectCallback(IUeGui::Callback callback)
{
    gui.setRejectCallback(callback);
}
void UserPort::showSmsComposerView()
{
    gui.setSmsComposeMode();
}


void UserPort::handleMenuSelection(unsigned index)
{
    switch (index)
    {
        case 0:
            handler->handleSmsCompose();

            break;
        case 1:
            handler->handleViewSmsList();
            break;
        case 2:
            //gui.setDialMode();
            showDialing();
            break;
        default:
            gui.setAlertMode().setText("Invalid selection");
            break;
    }
}

    void UserPort::showNewSms(bool)
{
    gui.setAlertMode().setText("New SMS received");
}

    void UserPort::showSmsCompose()
{
    gui.setSmsComposeMode();
}

}
