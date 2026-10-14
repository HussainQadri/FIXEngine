#pragma once

#include "TypedMessage.h"
#include <string>

class SequenceReset : public TypedMessage {
public:
    SequenceReset(const std::string& rawFixString);
    std::string msgType() const;
    std::string getGapFillFlag() const;
    std::string getNewSeqNo() const;
};
