#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);

    ~ConnectedState() override;

    void handleCallRequest(common::PhoneNumber phoneNumber) override;
    void handleUserAcceptCall()    override;
    void handleUserRejectCall()    override;
    void handleTimeout()           override;

    void handleDialAction() override;
    
private:
    bool waitingForCall{false};
    common::PhoneNumber callerNumber;
};

}
