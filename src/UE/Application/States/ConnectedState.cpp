#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "Sms.hpp"
#include "Context.hpp"
#include "Ports/IUserPort.hpp"
#include "Ports/UserPort.hpp"

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


    void ConnectedState::handleSms(common::PhoneNumber from, const std::string& text)
    {
        logger.logInfo("Received SMS from: " + common::to_string(from) + ", text: " + text);
        Sms sms{from, context.user.getOwnPhoneNumber(), text, true};
        context.smsDb.addMessage(sms);
        context.user.showNewSms(true);
        context.user.showSms(&sms);
    }

}
