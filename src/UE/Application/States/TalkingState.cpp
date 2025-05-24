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
    context.user.showAlert("Call ended");
    context.setState<ConnectedState>();
}

void TalkingState::handleUnknownRecipient(common::PhoneNumber /*phoneNumber*/)
{
    context.user.showAlert("Peer unavailable");
    context.setState<ConnectedState>();
}

void TalkingState::handleCallRequest(common::PhoneNumber)
{
}

}
