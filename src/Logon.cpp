#include "Logon.h"
#include "TypedMessage.h"

Logon::Logon(const string& rawFixString) : TypedMessage(rawFixString) {
    if (getValue("35") != "A") {
        throw std::invalid_argument("Not a Logon");
    }
}

std::string Logon::msgType() const {
    return "A";
}

std::string Logon::getHeartBtInt() const {
    return getValue("108");
}

std::string Logon::getEncryptMethod() const {
    return getValue("98");
}
std::string Logon::getRawDataLength() const {
    return getValue("95");
}
std::string Logon::getRawData() const {
    return getValue("96");
}
std::string Logon::getResetSeqNumFlag() const {
    return getValue("141");
}
std::string Logon::getNextExpectedMsgSeqNum() const {
    return getValue("789");
}
std::string Logon::getMaxMessageSize() const {
    return getValue("383");
}
std::string Logon::getNoMsgTypes() const {
    return getValue("384");
}
std::string Logon::getRefMsgType() const {
    return getValue("372");
}
std::string Logon::getMsgDirection() const {
    return getValue("385");
}
std::string Logon::getTestMessageIndicator() const {
    return getValue("464");
}
std::string Logon::getUsername() const {
    return getValue("553");
}
std::string Logon::getPassword() const {
    return getValue("554");
}
