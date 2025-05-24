#pragma once

#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"
#include "Ports/ITimerPort.hpp"

namespace ue
{

    class IEventsHandler : public IBtsEventsHandler,
                           public IUserEventsHandler,
                           public ITimerEventsHandler
    {
    public:
        virtual ~IEventsHandler() = default;

        virtual void handleSms(common::PhoneNumber from, const std::string& text) = 0;
    };

}
