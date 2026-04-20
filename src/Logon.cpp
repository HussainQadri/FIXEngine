#include "Logon.h"
#include "TypedMessage.h"

Logon::Logon(const string& rawFixString) : TypedMessage(rawFixString) {
}

std::string Logon::msgType() const {
    return "A";
}
