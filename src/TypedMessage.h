#pragma once
#include "FIXMessage.h"

class TypedMessage : public FIXMessage {
public:
    TypedMessage(const std::string& rawFixString) : FIXMessage(rawFixString) {
    }
    virtual ~TypedMessage() = default;
    virtual std::string msgType() const = 0;
    virtual bool validate() const = 0;
};
