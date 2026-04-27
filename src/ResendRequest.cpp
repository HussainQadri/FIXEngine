#include "ResendRequest.h"
#include "TypedMessage.h"

ResendRequest::ResendRequest(const string& rawFixString)
    : TypedMessage(rawFixString) {
    if (getValue("35") != "2") {
        throw std::invalid_argument("Not a ResendRequest");
    }
}

std::string ResendRequest::msgType() const {
    return "2";
}

std::string ResendRequest::getBeginSeqNo() const {
    return getValue("7");
}
std::string ResendRequest::getEndSeqNo() const {
    return getValue("16");
}
