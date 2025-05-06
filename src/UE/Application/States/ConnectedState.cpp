#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ConstructSmsState.hpp"
#include "Context.hpp"
#include "UserPort.hpp"
#include "BtsPort.hpp"

namespace ue
{
    ConnectedState::ConnectedState(Context& context)
        : BaseState(context, "ConnectedState")
    {
        context.user.showConnected();
    }

    void ConnectedState::handleDisconnected(){
        context.setState<NotConnectedState>();
    }

    void ConnectedState::handleConstructSms(common::PhoneNumber to, const std::string &text) {
        context.setState<ConstructSmsState>(to, text);
    }
}
