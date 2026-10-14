#pragma once
#include "TypedMessage.h"
#include <string>
class Logon : public TypedMessage {
public:
    Logon(const std::string& rawFixString);
    std::string getHeartBtInt() const;
    std::string msgType() const override;
    std::string getEncryptMethod() const;
    std::string getRawDataLength() const;
    std::string getRawData() const;
    std::string getResetSeqNumFlag() const;
    std::string getNextExpectedMsgSeqNum() const;
    std::string getMaxMessageSize() const;
    std::string getNoMsgTypes() const;
    std::string getRefMsgType() const;
    std::string getMsgDirection() const;
    std::string getTestMessageIndicator() const;
    std::string getUsername() const;
    std::string getPassword() const;
};
