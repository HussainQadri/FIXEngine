#pragma once
#include "TypedMessage.h"

class ResendRequest : public TypedMessage {
public:
    ResendRequest(const string& rawFixString);
    string msgType() const;
    string getBeginSeqNo() const;
    string getEndSeqNo() const;
};
