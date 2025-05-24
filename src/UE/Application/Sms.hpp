#pragma once
#include "Messages/PhoneNumber.hpp"
#include <string>
#include <vector>

namespace ue {

    struct Sms
    {
        common::PhoneNumber sender;
        common::PhoneNumber receiver;
        std::string text;
        bool isIncoming;
    };

    class SmsDatabase
    {
    public:
        void addMessage(const Sms& sms);
        const std::vector<Sms>& getMessages() const;

    private:
        std::vector<Sms> messages;
    };

}
