#include "ConstructSmsState.hpp"
#include "BtsPort.hpp"

namespace ue
{

    ConstructSmsState::ConstructSmsState(Context &context, common::PhoneNumber to, const std::string &text)
    : BaseState(context, "ConnectedState")
    {
        this->to = to;
        this->message = text;
    }

    void ConstructSmsState::handleConstructSms(common::PhoneNumber to, const std::string &text)
    {
        logger.logInfo("Send SMS to: " + common::to_string(to) + ", text: " + text);
        context.bts.sendSms(to, message);
    }

}
