#pragma once

#include "Messages/BtsId.hpp"
#include <Messages/PhoneNumber.hpp>

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleDisconnected() = 0;

    virtual void handleCallRequest(common::PhoneNumber) = 0;
    virtual void handleUserAcceptCall() = 0;
    virtual void handleUserRejectCall() = 0;
    virtual void handleUnknownRecipient(common::PhoneNumber phoneNumber) = 0;
    virtual void handleCallAccept() = 0;
    virtual void handleSmsReceived(const std::string&, common::PhoneNumber, common::PhoneNumber) = 0;
};

class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;

    virtual void sendCallRequest(common::PhoneNumber) = 0;
    virtual void sendCallAccept(common::PhoneNumber) = 0;
    virtual void sendCallDrop(common::PhoneNumber) = 0;
    virtual void sendCallReject(common::PhoneNumber) = 0;
    virtual void sendSms(common::PhoneNumber, const std::string&) = 0;
    virtual common::PhoneNumber getOwnPhoneNumber() = 0;

};

}
