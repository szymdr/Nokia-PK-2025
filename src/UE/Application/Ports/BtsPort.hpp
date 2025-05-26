#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
public:
    BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler& handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    void sendCallRequest(common::PhoneNumber) override;
    void sendCallAccept(common::PhoneNumber) override;
    void sendCallDrop(common::PhoneNumber) override;
    void sendCallReject(common::PhoneNumber) override;
    void sendSms(common::PhoneNumber, const std::string&) override;
    common::PhoneNumber getOwnPhoneNumber() override;

private:
    void handleMessage(BinaryMessage msg);

    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
