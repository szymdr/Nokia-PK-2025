#pragma once

#include <gmock/gmock.h>
#include <Messages/PhoneNumber.hpp>

#include "Ports/IBtsPort.hpp"

namespace ue
{

class IBtsEventsHandlerMock : public IBtsEventsHandler
{
public:
    IBtsEventsHandlerMock();
    ~IBtsEventsHandlerMock() override;

    MOCK_METHOD(void, handleSib, (common::BtsId), (final));
    MOCK_METHOD(void, handleAttachAccept, (), (final));
    MOCK_METHOD(void, handleAttachReject, (), (final));
    MOCK_METHOD(void, handleDisconnected, (), (final));

    MOCK_METHOD(void, handleCallRequest, (common::PhoneNumber), (override));
    MOCK_METHOD(void, handleUserAcceptCall, (), (override));
    MOCK_METHOD(void, handleUnknownRecipient, (common::PhoneNumber), (override));
    MOCK_METHOD(void, handleCallAccept, (), (override));

    MOCK_METHOD(void, handleSmsReceived, (const std::string&, common::PhoneNumber, common::PhoneNumber), (override));

    MOCK_METHOD(void, handleRemoteCallDrop, (), (override));
    MOCK_METHOD(void, handleBtsCallTalk, (const std::string &text), (override));


};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(void, sendCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallAccept, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallDrop, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallReject, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendSms, (common::PhoneNumber, const std::string&), (override));
    MOCK_METHOD(common::PhoneNumber, getOwnPhoneNumber, (), (override));

    MOCK_METHOD(void, sendCallTalk, (common::PhoneNumber to, const std::string &text), (override));



};

}
