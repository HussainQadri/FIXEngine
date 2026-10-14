#pragma once
#include "TypedMessage.h"
#include <string>

class ResendRequest : public TypedMessage {
public:
    ResendRequest(const std::string& rawFixString);
    std::string msgType() const;
    std::string getBeginSeqNo() const;
    std::string getEndSeqNo() const;
};
