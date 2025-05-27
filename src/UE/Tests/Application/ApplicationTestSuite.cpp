#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>
#include "Mocks/IUeGuiMock.hpp"
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
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
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
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
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


    struct ApplicationSendingSmsTestSuite : ApplicationConnectedTestSuite
{
    SmsDb dummyDb;
    std::unique_ptr<NiceMock<ISmsComposeModeMock>> composeMode;

    ApplicationSendingSmsTestSuite()
    {
        EXPECT_CALL(userPortMock, showSmsComposerView());

        composeMode = std::make_unique<NiceMock<ISmsComposeModeMock>>();
        EXPECT_CALL(userPortMock, getSmsComposeMode()).WillRepeatedly(ReturnRef(*composeMode));
        EXPECT_CALL(*composeMode, clearSmsText());
        testing::Mock::AllowLeak(composeMode.get());

        EXPECT_CALL(userPortMock, setAcceptCallback(_));
        EXPECT_CALL(userPortMock, setRejectCallback(_));

        objectUnderTest.handleSmsCompose();
    }
};

    TEST_F(ApplicationSendingSmsTestSuite, shallHandleSmsSend)
    {
        const auto PEER_NUMBER = common::PhoneNumber{123};
        const std::string TEXT = "Test SMS";

        EXPECT_CALL(*composeMode, getPhoneNumber()).WillOnce(Return(PEER_NUMBER));
        EXPECT_CALL(*composeMode, getSmsText()).WillOnce(Return(TEXT));
        EXPECT_CALL(userPortMock, getSmsDb()).WillOnce(ReturnRef(dummyDb));
        EXPECT_CALL(btsPortMock, getOwnPhoneNumber()).WillOnce(Return(PHONE_NUMBER));
        EXPECT_CALL(btsPortMock, sendSms(PEER_NUMBER, TEXT));
        EXPECT_CALL(userPortMock, showConnected());

        objectUnderTest.handleSmsSend();
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

    struct ApplicationViewingSmsListTestSuite : ApplicationConnectedTestSuite {
        ApplicationViewingSmsListTestSuite() {
            ON_CALL(userPortMock, getSmsDb()).WillByDefault(ReturnRef(dummyDb));
            EXPECT_CALL(userPortMock, showSmsList());
            objectUnderTest.handleViewSmsList();
        }
    };

    TEST_F(ApplicationViewingSmsListTestSuite, shallHandleViewSmsAccept) {
        EXPECT_CALL(userPortMock, getAction()).WillOnce(Return(0));
        EXPECT_CALL(userPortMock, showSms(0));
        objectUnderTest.handleViewSmsAccept();
    }

    TEST_F(ApplicationViewingSmsListTestSuite, shallHandleViewSmsClose) {
        EXPECT_CALL(userPortMock, showConnected());
        objectUnderTest.handleViewSmsClose();
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
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
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
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallDrop();
}

TEST_F(ApplicationDialingTestSuite, shallReturnToMenuOnRemoteCallDropped)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showAlert("Call not accepted"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleRemoteCallDrop();
}

TEST_F(ApplicationDialingTestSuite, ignoreUnknownRecipientAfterUserReject)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(btsPortMock, sendCallDrop(PEER_NUMBER));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleCallDrop();

    // BTS → UnknownRecipient
    EXPECT_NO_THROW(objectUnderTest.handleUnknownRecipient(PEER_NUMBER));
}

struct ApplicationTalkingTestSuite : ApplicationReceivingCallTestSuite
{
    ApplicationTalkingTestSuite()
    {
        using namespace std::chrono_literals;
        EXPECT_CALL(timerPortMock, stopTimer());
        EXPECT_CALL(btsPortMock, sendCallAccept(PHONE_NUMBER));
        EXPECT_CALL(userPortMock, showTalking());
        EXPECT_CALL(timerPortMock, startTimer(120000ms));
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
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showAlert("Call ended"));
    EXPECT_CALL(userPortMock, showConnected());
    objectUnderTest.handleRemoteCallDrop();
}

TEST_F(ApplicationTalkingTestSuite, shallSendMessageOnUserInput)
{
    const std::string TEXT = "Hello";
    EXPECT_CALL(btsPortMock, sendCallTalk(PHONE_NUMBER, TEXT));
    EXPECT_CALL(userPortMock, appendIncomingText("You: " + TEXT));
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, startTimer(120000ms));

    objectUnderTest.handleUserCallTalk(TEXT);
}

TEST_F(ApplicationTalkingTestSuite, shallReceiveMessageFromPeer)
{
    const std::string TEXT = "PeerMsg";
    EXPECT_CALL(userPortMock, appendIncomingText("Peer: " + TEXT));
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, startTimer(120000ms));

    objectUnderTest.handleBtsCallTalk(TEXT);
}

TEST_F(ApplicationTalkingTestSuite, shallDropCallAfterInactivityFollowingSend)
{
    const std::string TEXT = "Hi";
    EXPECT_CALL(btsPortMock, sendCallTalk(PHONE_NUMBER, TEXT));
    EXPECT_CALL(userPortMock, appendIncomingText("You: " + TEXT));
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest.handleUserCallTalk(TEXT);

    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showAlert("Call ended due to inactivity"));
    EXPECT_CALL(userPortMock, showConnected());

    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationTalkingTestSuite, shallDropCallAfterInactivityFollowingReceive)
{
    const std::string TEXT = "HiBack";
    EXPECT_CALL(userPortMock, appendIncomingText("Peer: " + TEXT));
    using namespace std::chrono_literals;
    EXPECT_CALL(timerPortMock, startTimer(120000ms));
    objectUnderTest.handleBtsCallTalk(TEXT);

    EXPECT_CALL(btsPortMock, sendCallDrop(PHONE_NUMBER));
    EXPECT_CALL(userPortMock, showAlert("Call ended due to inactivity"));
    EXPECT_CALL(userPortMock, showConnected());

    objectUnderTest.handleTimeout();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipientDuringTalking)
{
    EXPECT_CALL(userPortMock, showAlert("Peer unavailable"));
    EXPECT_CALL(userPortMock, showConnected());

    objectUnderTest.handleUnknownRecipient(PHONE_NUMBER);

}

}
