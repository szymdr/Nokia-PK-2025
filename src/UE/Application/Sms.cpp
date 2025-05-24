#include "Sms.hpp"

namespace ue {

    void SmsDatabase::addMessage(const Sms& sms)
    {
        messages.push_back(sms);
    }

    const std::vector<Sms>& SmsDatabase::getMessages() const
    {
        return messages;
    }

}
