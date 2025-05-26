#include "ConnectedState.hpp"
#include "SendingSmsState.hpp"

namespace ue {

    SendingSmsState::SendingSmsState(Context &context) : ConnectedState(
            context){
        context.user.showSmsComposerView();
        auto& composeMode = context.user.getSmsComposeMode();
        composeMode.clearSmsText();

        context.user.setAcceptCallback([this]() {
            this->handleSmsSend();
        });
    }

    void SendingSmsState::handleSmsSend() {
        auto& composeMode = context.user.getSmsComposeMode();
        PhoneNumber toPhoneNumber = composeMode.getPhoneNumber();
        std::string text = composeMode.getSmsText();

        SmsDb &db = context.user.getSmsDb();
        db.addSms(text, context.bts.getOwnPhoneNumber(), toPhoneNumber);
        context.bts.sendSms(toPhoneNumber, text);
        context.setState<ConnectedState>();
    }

}