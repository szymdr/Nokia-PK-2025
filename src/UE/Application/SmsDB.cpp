#include "SmsDB.hpp"

ue::SmsDb::SmsDb() {

}

void ue::SmsDb::addSms(const std::string& text, common::PhoneNumber fromPhoneNumber, common::PhoneNumber toPhoneNumber) {
    Sms sms = Sms(text, fromPhoneNumber, toPhoneNumber);
    smsList.push_back(sms);
}

std::vector<Sms> ue::SmsDb::getSmsList() {
    return smsList;
}

std::optional<Sms> ue::SmsDb::retrieveSms(int index) {
    if(smsList.size() > index) {
        smsList[index].markAsRead();
        return smsList[index];
    }
    return std::nullopt;
}
void ue::SmsDb::markOwnAsRead(){
    int lastIndex = smsList.size()-1;
    smsList[lastIndex].markAsRead();
}
