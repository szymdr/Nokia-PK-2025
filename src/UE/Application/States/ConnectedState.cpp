#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "ConstructSmsState.hpp"
#include "Sms.hpp"
#include "Context.hpp"

namespace ue
{

    ConnectedState::ConnectedState(Context& context)
        : BaseState(context, "ConnectedState")
    {
        context.user.showConnected();
    }

    void ConnectedState::handleDisconnected()
    {
        context.setState<NotConnectedState>();
    }

    void ConnectedState::handleConstructSms(common::PhoneNumber to, const std::string& text)
    {
        context.setState<ConstructSmsState>(to, text);
    }

    void ConnectedState::handleSms(common::PhoneNumber from, const std::string& text)
    {
        logger.logInfo("Received SMS from: " + common::to_string(from) + ", text: " + text);
        context.smsDb.addMessage({from, context.user.getOwnPhoneNumber(), text, true});
        context.user.showNewSms();
    }

}
