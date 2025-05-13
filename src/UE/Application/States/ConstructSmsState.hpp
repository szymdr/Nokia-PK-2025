#pragma once

#include "BaseState.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

    class ConstructSmsState : public BaseState
    {
    public:
        ConstructSmsState(Context& context, common::PhoneNumber to, const std::string& text);

    private:
        common::PhoneNumber to;
        std::string message;
    };

}
