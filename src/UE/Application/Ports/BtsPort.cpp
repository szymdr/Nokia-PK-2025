#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    this->handler = &handler;
    transport.registerDisconnectedCallback([this]{this->handler->handleDisconnected();});
}

void BtsPort::stop()
{
    transport.registerDisconnectedCallback(nullptr);
    transport.registerMessageCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
            {
                logger.logDebug("Attach accepted");
                handler->handleAttachAccept();
            }
            else
            {
                logger.logDebug("Attach rejected");
                handler->handleAttachReject();
            }
            break;
        }
        case common::MessageId::CallAccepted:
        {
            logger.logDebug("Call accepted from: ", from, ", to: ", to);
            handler->handleCallAccept();
            break;
        }
        case common::MessageId::CallRequest:
            handler->handleCallRequest(from);
            break;

        case common::MessageId::CallDropped:
        {
            logger.logDebug("CallDropped received from: ", from);
            handler->handleRemoteCallDrop();
            break;
        }

        case common::MessageId::UnknownRecipient:
        {
            handler->handleUnknownRecipient(to);
            break;
        }

        case common::MessageId::CallTalk:
        {
            auto text = reader.readRemainingText();
            logger.logDebug("CallTalk received from: ", from, ", text: ", text);
            handler->handleBtsCallTalk(text);
            break;
        }

        default:
            logger.logError("unknown message: ", msgId, ", from: ", from);

        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}


void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber number)
{
    logger.logDebug("sendCallRequest: from=", phoneNumber, ", to=", number);
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                phoneNumber,
                                number};
    msg.writePhoneNumber(number);
    transport.sendMessage(msg.getMessage());
}
void BtsPort::sendCallAccept(common::PhoneNumber number) {
    logger.logDebug("sendCallAccept: ", number);
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                phoneNumber,
                                number};
    msg.writePhoneNumber(number);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallDrop(common::PhoneNumber number)
{
    logger.logDebug("sendCallDrop: ", number);
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                phoneNumber,
                                number};
    msg.writePhoneNumber(number);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallReject(common::PhoneNumber number)
{
    logger.logDebug("sendCallReject: ", number);
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                phoneNumber,
                                number};
    msg.writePhoneNumber(number);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallTalk(common::PhoneNumber to, const std::string& text)
{
    logger.logDebug("Sending CallTalk to ", to, ": ", text);

    common::OutgoingMessage msg{common::MessageId::CallTalk, phoneNumber, to};
    msg.writeText(text);

    transport.sendMessage(msg.getMessage());
}


}
