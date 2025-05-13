#pragma once

#include "BaseState.hpp"

namespace ue
{

    class WaitingForCallAcceptState : public BaseState
    {
    public:
        WaitingForCallAcceptState(Context& context, common::PhoneNumber phoneNumber);
        ~WaitingForCallAcceptState() override;

        void handleCallAccept() override;
        void handleCallDrop() override;
        void handleDialAction() override;
        void handleTimeout() override;
    private:
        common::PhoneNumber phoneNumber;
    };

}