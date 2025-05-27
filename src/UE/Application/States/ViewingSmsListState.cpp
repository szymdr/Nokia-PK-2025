#include "ViewingSmsListState.hpp"
#include "ViewingSmsState.hpp"
#include <sstream>

#include "ConnectedState.hpp"


namespace ue {


    ViewingSmsListState::ViewingSmsListState(Context &context) : ConnectedState(context) {
        context.user.showSmsList();
    }

    void ViewingSmsListState::handleViewSmsAccept() {
        int index = context.user.getAction();
        context.setState<ViewingSmsState>(index);
    }

    void ViewingSmsListState::handleViewSmsClose() {
        context.setState<ConnectedState>();
    }

}