#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() override;

    ~ConnectedState() override;
    void handleDialAction() override;
    void handleCallRequest(common::PhoneNumber callerNumber) override;
    void handleSmsReceived(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) override;
    void handleSmsCompose() override;
    void handleViewSmsList() override;
};

}
