#pragma once
#include "TypedMessage.h"

class Heartbeat : TypedMessage {
public:
    std::string msgType() const override;
    bool validate() const override;
};
