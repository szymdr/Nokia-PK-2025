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
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
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

void UserPort::showIncomingCall(common::PhoneNumber number)
{
    auto& callMode = gui.setCallMode();
    callMode.clearIncomingText();
    callMode.appendIncomingText("Incoming call from: " + to_string(number));

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
            handler->handleDialAction();
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
        //gui.setDialMode();
        showDialing();
        break;
    default:
        gui.setAlertMode().setText("Invalid selection");
        break;
    }
}

void UserPort::setAcceptCallback(std::function<void()> callback)
{
    gui.setAcceptCallback(callback);
}

void UserPort::setRejectCallback(std::function<void()> callback)
{
    gui.setRejectCallback(callback);
}

}
