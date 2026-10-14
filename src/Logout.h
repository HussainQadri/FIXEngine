#pragma once
#include "TypedMessage.h"
#include <string>

class Logout : public TypedMessage {
public:
    Logout(const std::string& rawFixString);
    std::string msgType() const;
    std::string getText() const;
    std::string getEncodedTextLen() const;
    std::string getEncodedText() const;
};
