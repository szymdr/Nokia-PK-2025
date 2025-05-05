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

    MOCK_METHOD(void, handleCallRequest, (common::PhoneNumber), (override));
    MOCK_METHOD(void, handleUserAcceptCall, (), (override));
    MOCK_METHOD(void, handleUserRejectCall, (), (override));
};

class IBtsPortMock : public IBtsPort
{
public:
    IBtsPortMock();
    ~IBtsPortMock() override;

    MOCK_METHOD(void, sendAttachRequest, (common::BtsId), (final));
    MOCK_METHOD(void, sendCallAccept, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallDrop, (common::PhoneNumber), (final));
    MOCK_METHOD(void, sendCallReject, (common::PhoneNumber), (final));

};

}
