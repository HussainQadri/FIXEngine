#include "Heartbeat.h"

std::string Heartbeat::msgType() const {
    return "0";
}

bool Heartbeat::validate() const {
    // will actually implement, validate will check if header and trailer
    // contains the required fields for header and trailer.
    return true;
}
