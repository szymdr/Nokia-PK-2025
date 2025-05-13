#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Unexpected: handleTimeout");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Unexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Unexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Unexpected: handleAttachReject");
}

void BaseState::handleCallRequest(common::PhoneNumber)
{
    logger.logError("Unexpected: handleCallRequest");
}
void BaseState::handleUserAcceptCall()
{
    logger.logError("Unexpected: handleUserAcceptCall");
}
void BaseState::handleUserRejectCall()
{
    logger.logError("Unexpected: handleUserRejectCall");
}
void BaseState::handleUnknownRecipient(common::PhoneNumber phoneNumber)
{
    logger.logError("Unexpected: handleUnknownRecipient: ", phoneNumber);
}
void BaseState::handleDialAction()
{
    logger.logError("Unexpected: handleDialAction");
}
void BaseState::handleCallAccept()
{
    logger.logError("Unexpected: handleCallAccept");
}
void BaseState::handleCallDrop()
{
    logger.logError("Unexpected: handleCallDrop");
}
void BaseState::handleDisconnected(){
    logger.logError("Uexpected: handleDisconnected");
}
}
