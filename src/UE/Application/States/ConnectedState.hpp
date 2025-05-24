#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class ConnectedState : public BaseState
    {
    public:
        ConnectedState(Context& context);

        void handleDisconnected() override;
        void handleSms(common::PhoneNumber from, const std::string& text) override;
    };

}