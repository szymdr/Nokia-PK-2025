#pragma once

#include "BaseState.hpp"

namespace ue
{

    class DialingState : public BaseState
    {
    public:
        DialingState(Context& context, const common::PhoneNumber& phoneNumber);
        ~DialingState() override;

        void handleUserAcceptCall() override;
        void handleUserRejectCall() override;

        void handleTimeout() override;
        void onEnter() override;
        void handleDialAction() override;

        void handleCallAccept();
    private:
        common::PhoneNumber phoneNumber;
    };

}