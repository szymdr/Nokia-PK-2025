#pragma once
#include <Messages/PhoneNumber.hpp>
#include <chrono>

class Sms {
private:
    std::string text;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool read = false;
    bool isFailedVar = false;

public:
    Sms();
    Sms(const std::string &text, const common::PhoneNumber &from, const common::PhoneNumber &to);

    void markAsRead();
    bool isFailed() const;
    void markAsFailed();
    bool isRead() const;
    std::string getText()const;

    const common::PhoneNumber &getFrom() const;

    const common::PhoneNumber &getTo() const;

};