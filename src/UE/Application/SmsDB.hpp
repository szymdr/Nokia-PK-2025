#pragma once
#include <Sms.hpp>
#include <vector>
#include <string>
#include <optional>

namespace ue {

    class SmsDb {

    private:
        std::vector<Sms> smsList;
    public:
        SmsDb();
        void addSms(const std::string&, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber);
        std::vector<Sms> getSmsList();
        std::optional<Sms> retrieveSms(int index);
        void markOwnAsRead();
    };
}