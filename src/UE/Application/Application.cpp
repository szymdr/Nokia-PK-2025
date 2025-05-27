#include "Application.hpp"
#include "States/NotConnectedState.hpp"
#include "States/ConnectedState.hpp"
#include "States/ViewingSmsListState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer)
    : context{iLogger, bts, user, timer},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}
void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleCallRequest(common::PhoneNumber phoneNumber)
{
    context.state->handleCallRequest(phoneNumber);
}
void Application::handleUserAcceptCall()
{
    context.state->handleUserAcceptCall();
}

void Application::handleUnknownRecipient(common::PhoneNumber phoneNumber)
{
    context.state->handleUnknownRecipient(phoneNumber);
}

void Application::handleDialAction()
{
    context.state->handleDialAction();
}

 void Application::handleCallAccept()
{
    context.state->handleUserAcceptCall();
}

void Application::handleCallDrop()
{
    context.state->handleCallDrop();
}

void Application::handleRemoteCallDrop(common::PhoneNumber phoneNumber)
    {
    context.state->handleRemoteCallDrop(phoneNumber);
}
void Application::handleUserCallTalk(const std::string& text)
{
    context.state->handleUserCallTalk(text);
}

void Application::handleBtsCallTalk(const std::string& text)
{
    context.state->handleBtsCallTalk(text);
}
void Application::handleSmsReceived(const std::string &text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    context.state->handleSmsReceived(text, fromPhoneNumber, toPhoneNumber);
}

void Application::handleSmsCompose() {
    context.state->handleSmsCompose();
}

void Application::handleViewSmsList() {
    context.state->handleViewSmsList();
}

void Application::handleViewSmsAccept() {
    context.state->handleViewSmsAccept();
}

void Application::handleViewSmsClose() {
    context.state->handleViewSmsClose();
}

void Application::handleSmsSend() {
    context.state->handleSmsSend();
}
void Application::SendSms() {
    context.state->SendSms();
}
}

