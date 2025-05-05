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
}

void UserPort::showIncomingCall(common::PhoneNumber number)
{
    std::string msg = gui.setCallMode().getOutgoingText();
}

void UserPort::showDialing()
{
    common::PhoneNumber number = gui.setDialMode().getPhoneNumber();
}

void UserPort::showTalking()
{
    gui.setCallMode().appendIncomingText("Talking...");
}

void UserPort::showAlert(const std::string &text)
{
    gui.setAlertMode().setText("" + text);

}

}
