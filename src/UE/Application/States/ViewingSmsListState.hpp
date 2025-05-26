#pragma once
#include "ConnectedState.hpp"


namespace ue {

    class ViewingSmsListState : public ConnectedState {
    public:
        ViewingSmsListState(Context& context);
        void handleViewSmsAccept() override;
        void handleViewSmsClose() override;
    };

}