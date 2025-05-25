#include "TalkingState.hpp"

#include "ConnectedState.hpp"
#include "Context.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context, const common::PhoneNumber &talkingToPhoneNumber)
  : BaseState(context, "TalkingState"), talkingToPhoneNumber(talkingToPhoneNumber)
{
    context.user.showTalking();
}

TalkingState::~TalkingState() = default;

void TalkingState::handleCallDrop()
{;
    context.bts.sendCallDrop(talkingToPhoneNumber);

    context.setState<ConnectedState>();
}

void TalkingState::handleRemoteCallDrop()
{
    context.setState<ConnectedState>();
    context.user.showAlert("Call ended");
}

void TalkingState::handleUnknownRecipient(common::PhoneNumber /*phoneNumber*/)
{
    context.setState<ConnectedState>();
    context.user.showAlert("Peer unavailable");
}

void TalkingState::handleCallRequest(common::PhoneNumber)
{
}

}
