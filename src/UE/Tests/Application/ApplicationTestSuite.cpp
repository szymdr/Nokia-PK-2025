#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

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
    ApplicationConnectedTestSuite();
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite() {
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showConnected());
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
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
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
    objectUnderTest.handleCallDrop();
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
    objectUnderTest.handleCallDrop();

    // BTS → UnknownRecipient
    EXPECT_NO_THROW(objectUnderTest.handleUnknownRecipient(PHONE_NUMBER));
}

struct ApplicationDialingTestSuite : ApplicationConnectedTestSuite
{
    const common::PhoneNumber PEER_NUMBER{123};

    ApplicationDialingTestSuite()
    {
        using namespace std::chrono_literals;
        EXPECT_CALL(userPortMock, getDialedPhoneNumber()).WillRepeatedly(Return(PEER_NUMBER));
        EXPECT_CALL(userPortMock, setDialNumber(PEER_NUMBER));
        EXPECT_CALL(btsPortMock, sendCallRequest(PEER_NUMBER));
        EXPECT_CALL(timerPortMock, startTimer(60000ms));
        EXPECT_CALL(userPortMock, showCalling(PEER_NUMBER));
        objectUnderTest.handleDialAction();
    }
};

TEST_F(ApplicationDialingTestSuite, shallShowTalkingOnAccept)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showTalking());
    objectUnderTest.handleUserAcceptCall();
}

TEST_F(ApplicationDialingTestSuite, shallReturnToMenuOnUnknownRecipient)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showAlert("Peer UE not connected"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleUnknownRecipient(PEER_NUMBER);
}

TEST_F(ApplicationDialingTestSuite, shallReturnToMenuOnTimeout)
{
    EXPECT_CALL(userPortMock, showAlert("Call timeout"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationDialingTestSuite, shallSendCallDropOnUserReject)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDrop(PEER_NUMBER));
    EXPECT_CALL(userPortMock, showAlert("Call dropped"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallDrop();
}

struct ApplicationTalkingTestSuite : ApplicationReceivingCallTestSuite
{
    ApplicationTalkingTestSuite()
    {
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
        EXPECT_CALL(userPortMock, showTalking());
        objectUnderTest.handleUserAcceptCall();
    }
};

TEST_F(ApplicationTalkingTestSuite, shallDropCallOnUserAction)
{
    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallDrop();
}

TEST_F(ApplicationTalkingTestSuite, shallEndCallOnRemoteDrop)
{
    EXPECT_CALL(userPortMock, showAlert("Call ended"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleRemoteCallDrop();
}


}
