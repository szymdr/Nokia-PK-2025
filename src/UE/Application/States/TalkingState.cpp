#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
  : BaseState(context, "TalkingState")
{
    context.user.showTalking();
}

TalkingState::~TalkingState() = default;

}