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
