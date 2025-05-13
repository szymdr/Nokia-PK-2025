#include "ConstructSmsState.hpp"
#include "ConnectedState.hpp"
#include "Context.hpp"
#include "Sms.hpp"

namespace ue
{

    ConstructSmsState::ConstructSmsState(Context& context, common::PhoneNumber to, const std::string& text)
        : BaseState(context, "ConstructSmsState"), to(to), message(text)
    {
        logger.logInfo("Sending SMS to: " + common::to_string(to) + ", text: " + message);

        context.bts.sendSms(to, message);

        context.smsDb.addMessage({
            .sender = context.user.getOwnPhoneNumber(),
            .receiver = to,
            .text = message,
            .isReceived = false
        });

        context.setState<ConnectedState>();
    }

}
