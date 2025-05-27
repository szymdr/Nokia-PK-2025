#include "ViewingSmsState.hpp"
#include "ViewingSmsListState.hpp"
#include <sstream>

#include "ConnectedState.hpp"


namespace ue {
    ViewingSmsState::ViewingSmsState(Context& context, int index)
            : ConnectedState(context) {
        context.user.showSms(index);
    }

    void ViewingSmsState::handleViewSmsAccept() {
        context.setState<ConnectedState>();
    }

    void ViewingSmsState::handleViewSmsClose() {
        context.setState<ViewingSmsListState>();
    }

}