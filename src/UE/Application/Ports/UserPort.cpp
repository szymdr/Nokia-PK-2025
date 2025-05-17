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

void UserPort::showTalking()
{
    gui.setCallMode().clearIncomingText();
    gui.setCallMode().appendIncomingText("Talking...");
}

void UserPort::showAlert(const std::string &text)
{
    gui.setAlertMode().setText("" + text);

}

}
