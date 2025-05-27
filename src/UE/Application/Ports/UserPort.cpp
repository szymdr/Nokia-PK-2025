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
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Incoming call from: \n" + to_string(callerNumber));

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
    auto &callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.appendIncomingText("Type phone number:");
    auto &dialMode = gui.setDialMode();

    gui.setAcceptCallback([this, &dialMode]() {
        if (handler)
        {
            dialedPhoneNumber = dialMode.getPhoneNumber();
            logger.logDebug("UserPort: Dial number set to: ", dialedPhoneNumber);
            handler->handleDialAction();
        }
    });

    gui.setRejectCallback([this]() {
        showConnected();
    });
}

void UserPort::showCalling(const common::PhoneNumber& number)
{
    auto& alertMode = gui.setAlertMode();
    alertMode.setText("Dialing to: \n" + to_string(dialedPhoneNumber));

    gui.setAcceptCallback([this]() {});

    gui.setRejectCallback([this]() {

        if (handler)
        {
            handler->handleCallDrop();
        }
    });

}

void UserPort::showTalking()
{
    auto& callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.appendIncomingText("Talking...");

    gui.setAcceptCallback([this, &callMode]() {
        if (handler)
        {
            std::string text = callMode.getOutgoingText();
            if (!text.empty())
            {
                callMode.clearOutgoingText();
                handler->handleUserCallTalk(text);
            }
        }
    });

    gui.setRejectCallback([this]() {
        if (handler)
        {
            handler->handleCallDrop();
        }
    });
}

void UserPort::showAlert(const std::string &text)
{
    auto &alertMode =  gui.setAlertMode();
    alertMode.setText(text);

    gui.setAcceptCallback([this]() {
        showConnected();
    });

    gui.setRejectCallback([this]() {
        showConnected();
    });
}

common::PhoneNumber UserPort::getDialedPhoneNumber() const
{
    return dialedPhoneNumber;
}

void UserPort::setDialNumber(const common::PhoneNumber& number)
{

    gui.setDialMode();
    logger.logDebug("UserPort: Dial number set to: ", dialedPhoneNumber);

}



void UserPort::showSmsList() {
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();

    for (auto&& sms : smsDb.getSmsList()) {
        std::string isNew;
        if(!sms.isRead()){
            isNew = "NEW ";
        }
        std::string info = isNew + "FROM: " + common::to_string(sms.getFrom()) + "TO: " + common::to_string(sms.getTo());
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
            showDialing();
            break;
        default:
            gui.setAlertMode().setText("Invalid selection");
            break;
    }
}



void UserPort::appendIncomingText(const std::string& text)
{
    auto& callMode = gui.setCallMode();
    callMode.appendIncomingText(text);
    logger.logDebug("Appending incoming text: ", text);
}


}
