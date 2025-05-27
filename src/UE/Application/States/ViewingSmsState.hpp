#pragma once
#include "ConnectedState.hpp"

namespace  ue {
    class ViewingSmsState : public ConnectedState {
    public:
        ViewingSmsState(Context& context, int index);
        void handleViewSmsAccept() override;
        void handleViewSmsClose() override;
    };
};