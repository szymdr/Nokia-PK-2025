#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"


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


    void UserPort::showNewSms(bool present)
    {
        gui.showNewSms(present);
    }

    void UserPort::showSms(const Sms* sms)
    {
        IUeGui::ITextMode& mode = gui.setViewTextMode();
        mode.setText("From: " + common::to_string(sms->sender) + "\nText: " + sms->text);
    }




}
