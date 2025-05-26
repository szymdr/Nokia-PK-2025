#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "SmsDB.hpp"
#include "Sms.hpp"
namespace ue
{

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;

    void showIncomingCall(const common::PhoneNumber callerNumber) override;
    void showDialing() override;
    void showTalking() override;
    void showAlert(const std::string& text) override;

    common::PhoneNumber getDialedPhoneNumber() const override;

    void setDialNumber(const common::PhoneNumber& number);

    void handleMenuSelection(unsigned index);

    void showSmsList() override;
    void showSms(int index) override;
    int getAction() override;
    IUeGui::ISmsComposeMode& getSmsComposeMode() override;
    SmsDb& getSmsDb();
    void setAcceptCallback(IUeGui::Callback) override;
    void setRejectCallback(IUeGui::Callback) override;
    void showSmsComposerView() override;
private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber callerNumber;
    IUserEventsHandler* handler = nullptr;
    common::PhoneNumber dialedPhoneNumber;
    IUeGui::Callback callback;
    SmsDb smsDb;
    int action = -1;
    void onAcceptSmsList(IUeGui::IListViewMode& menu);

};

}
