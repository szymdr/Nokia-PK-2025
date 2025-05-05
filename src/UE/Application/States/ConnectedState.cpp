#include "ConnectedState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

    ConnectedState::~ConnectedState() = default;

void ConnectedState::handleCallRequest(common::PhoneNumber number)
{
    //context.userPort.showIncomingCall(number);
}

}
