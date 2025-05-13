#pragma once
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class IUserEventsHandler
    {
    public:
        virtual ~IUserEventsHandler() = default;
        virtual void handleConstructSms(common::PhoneNumber to, const std::string &text) = 0;
    };

    class IUserPort
    {
    public:
        virtual ~IUserPort() = default;

        virtual void showNotConnected() = 0;
        virtual void showConnecting() = 0;
        virtual void showConnected() = 0;

        virtual void showNewSms() = 0;
        virtual common::PhoneNumber getOwnPhoneNumber() const = 0;
    };

}
