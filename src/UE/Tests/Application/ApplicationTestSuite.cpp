#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>
#include <UeGui/ISmsComposeMode.hpp>
#include <SmsDB.hpp>

namespace ue
{
using namespace ::testing;

struct ApplicationTestSuite : Test
{
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{1024};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;

    Expectation showNotConnected = EXPECT_CALL(userPortMock, showNotConnected());
    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite {
    void sendAttachRequestOnSib();
};

void ApplicationNotConnectedTestSuite::sendAttachRequestOnSib()
 {
     using namespace std::chrono_literals;
     EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
     EXPECT_CALL(timerPortMock, startTimer(500ms));
     EXPECT_CALL(userPortMock, showConnecting());
     objectUnderTest.handleSib(BTS_ID);
 }

 TEST_F(ApplicationNotConnectedTestSuite, shallSendAttachRequestOnSib)
 {
     sendAttachRequestOnSib();
 }

 struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
 {
     ApplicationConnectingTestSuite()
     {
         sendAttachRequestOnSib();
     }
 };

TEST_F(ApplicationConnectingTestSuite, shallConnectOnAttachAccept)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleAttachAccept();
}

    TEST_F(ApplicationConnectingTestSuite, shallDisConnectOnAttachReject)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleAttachReject();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    SmsDb dummyDb;
    ApplicationConnectedTestSuite();
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite() {
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected()).Times(AnyNumber());
    objectUnderTest.handleAttachAccept();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnect) 
{
EXPECT_CALL(userPortMock, showNotConnected());
objectUnderTest.handleDisconnected();
}

    TEST_F(ApplicationConnectingTestSuite, shallDisConnectOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedCallTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedCallTestSuite()
    {
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(userPortMock, showConnected());
        objectUnderTest.handleAttachAccept();
    }
    void showIncomingCallAndStartTimeoutOnCallRequest();
};

void ApplicationConnectedCallTestSuite::showIncomingCallAndStartTimeoutOnCallRequest()
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showIncomingCall(PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(3000ms));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedCallTestSuite, shallShowIncomingCallAndStartTimeoutOnCallRequest)
{
    showIncomingCallAndStartTimeoutOnCallRequest();
}

struct ApplicationReceivingCallTestSuite : ApplicationConnectedCallTestSuite
{
    ApplicationReceivingCallTestSuite()
    {
        showIncomingCallAndStartTimeoutOnCallRequest();
    }
};

TEST_F(ApplicationReceivingCallTestSuite, shallAcceptCallOnUserAction)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showTalking());
    objectUnderTest.handleUserAcceptCall();
}

TEST_F(ApplicationReceivingCallTestSuite, shallRejectCallOnUserAction)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleUserRejectCall();
}

TEST_F(ApplicationReceivingCallTestSuite, shallRejectCallOnTimeout)
{
    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationReceivingCallTestSuite, shallAlertUserOnUnknownPeerAfterAccept)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showTalking());
    objectUnderTest.handleUserAcceptCall();

    // BTS → UnknownRecipient
    EXPECT_CALL(userPortMock, showAlert("Peer unavailable"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleUnknownRecipient(PHONE_NUMBER);
}

TEST_F(ApplicationReceivingCallTestSuite, ignoreUnknownPeerAfterReject)
{

    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleUserRejectCall();

    // BTS → UnknownRecipient
    EXPECT_NO_THROW(objectUnderTest.handleUnknownRecipient(PHONE_NUMBER));
}

    class MockComposeMode : public IUeGui::ISmsComposeMode
{
public:
    MOCK_METHOD(common::PhoneNumber, getPhoneNumber, (), (const, override));
    MOCK_METHOD(std::string, getSmsText, (), (const, override));
    MOCK_METHOD(void, clearSmsText, (), (override));
};

    TEST_F(ApplicationConnectedTestSuite, shallHandleSmsCompose)
{
    EXPECT_CALL(userPortMock, showSmsComposerView());
    auto composeMode = std::make_unique<NiceMock<MockComposeMode>>();
    EXPECT_CALL(userPortMock, getSmsComposeMode())
        .WillOnce(ReturnRef(*composeMode));
    EXPECT_CALL(*composeMode, clearSmsText());
    testing::Mock::AllowLeak(composeMode.get());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleSmsCompose();
}


    TEST_F(ApplicationConnectedTestSuite, shallHandleSmsSend)
{
    objectUnderTest.handleSmsSend();
    SUCCEED();
}


    TEST_F(ApplicationConnectedTestSuite, shallHandleSmsReceived)
    {
        const std::string TEXT = "Hello from test";
        const common::PhoneNumber SENDER{200};
        SmsDb tempDb;
        EXPECT_CALL(userPortMock, getSmsDb()).WillOnce(ReturnRef(tempDb));
        EXPECT_CALL(userPortMock, showAlert("NEW MESSAGE!"));
        objectUnderTest.handleSmsReceived(TEXT, SENDER, PHONE_NUMBER);
    }


    TEST_F(ApplicationConnectedTestSuite, shallHandleViewSmsList)
{
    SmsDb dummyDb;
    ON_CALL(userPortMock, getSmsDb()).WillByDefault(ReturnRef(dummyDb));
    EXPECT_CALL(userPortMock, showSmsList());
    objectUnderTest.handleViewSmsList();
}

    TEST_F(ApplicationConnectedTestSuite, shallHandleViewSmsAccept)
{
    objectUnderTest.handleViewSmsAccept();
    SUCCEED();
}

    TEST_F(ApplicationConnectedTestSuite, shallHandleViewSmsClose)
{
    objectUnderTest.handleViewSmsClose();
    SUCCEED();
}

}
