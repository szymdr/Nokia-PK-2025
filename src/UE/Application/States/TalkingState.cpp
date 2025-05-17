#include "TalkingState.hpp"

#include "ConnectedState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
  : BaseState(context, "TalkingState")
{
    context.user.showTalking();
}

TalkingState::~TalkingState() = default;

void TalkingState::handleUnknownRecipient(common::PhoneNumber /*phoneNumber*/)
{
    context.user.showAlert("Peer unavailable");
    context.setState<ConnectedState>();
}

}
