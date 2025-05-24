#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
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

        void showNewSms(bool present) override;
        void showSms(const Sms* sms) override;

        common::PhoneNumber getOwnPhoneNumber() const override;

    private:
        common::PrefixedLogger logger;
        IUeGui& gui;
        common::PhoneNumber phoneNumber;
        IUserEventsHandler* handler = nullptr;
    };

}
