#include "TalkingState.hpp"

namespace ue
{

TalkingState::TalkingState(Context &context)
  : BaseState(context, "TalkingState")
{
}

TalkingState::~TalkingState() = default;

}