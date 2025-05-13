#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    ~ConnectedState() override;

    void handleCallRequest(common::PhoneNumber callerNumber) override;
};

}
