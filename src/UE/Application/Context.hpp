#pragma once

#include "IEventsHandler.hpp"
#include "Logger/ILogger.hpp"
#include <memory>
#include "Sms.hpp"

namespace ue
{

struct Context
{
    common::ILogger& logger;
    IBtsPort& bts;
    IUserPort& user;
    ITimerPort& timer;
    std::unique_ptr<IEventsHandler> state{};
    SmsDatabase smsDb;

    template <typename State, typename ...Arg>
    void setState(Arg&& ...arg)
    {
        state = std::make_unique<State>(*this, std::forward<Arg>(arg)...);
    }
};

}
