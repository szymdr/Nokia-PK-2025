#pragma once

#include <gmock/gmock.h>
#include <Messages/PhoneNumber.hpp>

#include "Ports/IUserPort.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

    MOCK_METHOD(void, handleDialAction, (), (override));
    MOCK_METHOD(void, handleCallAccept, (), (override));
    MOCK_METHOD(void, handleCallDrop, (), (override));
    MOCK_METHOD(void, handleUserCallTalk, (const std::string &text), (override));


};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showIncomingCall, (common::PhoneNumber), (final));
    MOCK_METHOD(void, showDialing, (), (final));
    MOCK_METHOD(void, showCalling, (const common::PhoneNumber& number), (final));
    MOCK_METHOD(void, showTalking, (), (final));
    MOCK_METHOD(void, showAlert, (const std::string&), (final));
    MOCK_METHOD(void, appendIncomingText, (const std::string &text), (override));


    MOCK_METHOD(common::PhoneNumber, getDialedPhoneNumber, (), (const, final));
    MOCK_METHOD(void, setDialNumber, (const common::PhoneNumber& number), (final));

};

}
