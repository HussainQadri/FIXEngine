#include "Logout.h"
#include "TypedMessage.h"

Logout::Logout(const string& rawFixString) : TypedMessage(rawFixString) {
    if (getValue("35") != "5") {
        throw std::invalid_argument("Not a Logout");
    }
}

std::string Logout::msgType() const {
    return "5";
}

std::string Logout::getText() const {
    return getValue("58");
}
std::string Logout::getEncodedTextLen() const {
    return getValue("354");
}
std::string Logout::getEncodedText() const {
    return getValue("355");
}
