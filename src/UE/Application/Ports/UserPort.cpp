#include "UserPort.hpp"

#include <UeGui/ICallMode.hpp>
#include <UeGui/IDialMode.hpp>
#include <UeGui/ITextMode.hpp>

#include "UeGui/IListViewMode.hpp"

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

    gui.setAcceptCallback([this]() {});

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


void UserPort::handleMenuSelection(unsigned index)
{
    switch (index)
    {
    case 0:
        gui.setSmsComposeMode();
        break;
    case 1:
        gui.setListViewMode();
        break;
    case 2:
        showDialing();
        break;
    default:
        gui.setAlertMode().setText("Invalid selection");
        break;
    }
}

}
