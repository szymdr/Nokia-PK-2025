#include <chrono>
#include "Sms.hpp"

Sms::Sms() {
    text = "text";
}

Sms::Sms(const std::string &text, const common::PhoneNumber &from, const common::PhoneNumber &to) : text(text), from(from), to(to){}

void Sms::markAsFailed() {
    isFailedVar = true;
}
void Sms::markAsRead(){
    read = true;
}

bool Sms::isFailed() const {
    return isFailedVar;
}
bool Sms::isRead() const {
    return read;
}

std::string Sms::getText() const {
    return text;
}

const common::PhoneNumber &Sms::getFrom() const {
    return from;
}

const common::PhoneNumber &Sms::getTo() const {
    return to;
}
