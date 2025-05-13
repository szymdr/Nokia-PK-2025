#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

    using common::PhoneNumber;
    using common::ILogger;

    class Application : public IEventsHandler
    {
    public:
        Application(PhoneNumber phoneNumber,
                    ILogger& iLogger,
                    IBtsPort& bts,
                    IUserPort& user,
                    ITimerPort& timer);
        ~Application();

        void handleTimeout() override;
        void handleSib(common::BtsId btsId) override;
        void handleAttachAccept() override;
        void handleAttachReject() override;
        void handleDisconnected() override;
        void handleConstructSms(common::PhoneNumber to, const std::string& text) override;
        void handleSms(common::PhoneNumber from, const std::string& text) override;

    private:
        Context context;
        common::PrefixedLogger logger;
    };

}