#pragma once
#include "Messages/PhoneNumber.hpp"
#include "Sms.hpp"

namespace ue
{

    class IUserEventsHandler
    {
    public:
        virtual ~IUserEventsHandler() = default;

     };

    class IUserPort
    {
    public:
        virtual ~IUserPort() = default;

        virtual void showNotConnected() = 0;
        virtual void showConnecting() = 0;
        virtual void showConnected() = 0;

        virtual void showNewSms(bool present) = 0;
        virtual void showSms(const Sms* sms) = 0;
        virtual void handleSms(common::PhoneNumber from, const std::string& text) = 0;

        virtual common::PhoneNumber getOwnPhoneNumber() const = 0;

    };

}
