#pragma once
#include "TypedMessage.h"
class Logon : public TypedMessage {
public:
    Logon(const std::string& rawFixString);
    string getHeartBtInt() const;
    string msgType() const override;
    string getEncryptMethod() const;
    string getRawDataLength() const;
    string getRawData() const;
    string getResetSeqNumFlag() const;
    string getNextExpectedMsgSeqNum() const;
    string getMaxMessageSize() const;
    string getNoMsgTypes() const;
    string getRefMsgType() const;
    string getMsgDirection() const;
    string getTestMessageIndicator() const;
    string getUsername() const;
    string getPassword() const;
};
