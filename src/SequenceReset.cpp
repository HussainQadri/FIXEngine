#include "SequenceReset.h"
#include "TypedMessage.h"

SequenceReset::SequenceReset(const string& rawFixString)
    : TypedMessage(rawFixString) {
    if (getValue("35") != "4") {
        throw std::invalid_argument("Not a SequenceReset");
    }
}

std::string SequenceReset::msgType() const {
    return "4";
}

std::string SequenceReset::getGapFillFlag() const {
    return getValue("123");
}

std::string SequenceReset::getNewSeqNo() const {
    return getValue("36");
}
