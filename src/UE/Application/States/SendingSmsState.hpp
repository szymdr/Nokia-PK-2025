#pragma once

#include "ConnectedState.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue {
    class SendingSmsState : public ConnectedState {

    public:
        SendingSmsState(Context& context);

        void handleSmsSend() override;

    };
}