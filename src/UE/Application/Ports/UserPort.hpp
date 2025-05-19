#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"

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

    void setAcceptCallback(std::function<void()> callback) override;
    void setRejectCallback(std::function<void()> callback) override;

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber callerNumber;
    IUserEventsHandler* handler = nullptr;
    common::PhoneNumber dialedPhoneNumber;
};

}
