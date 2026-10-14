#include "FIXDictionary.h"
#include "FIXMessage.h"
#include <iomanip>
#include <iostream>
#include <string>
using std::string;

int main() {
    const string raw = "8=FIX.4.2\x01"
                       "9=118\x01"
                       "35=D\x01"
                       "49=ONIXS\x01"
                       "56=CME\x01"
                       "34=2\x01"
                       "52=20240528-09:20:52.111\x01"
                       "11=983532-3\x01"
                       "21=1\x01"
                       "38=100\x01"
                       "55=NVDA\x01"
                       "40=1\x01"
                       "54=1\x01"
                       "60=20240528-09:20:52.004\x01"
                       "10=017\x01";

    FIXDictionary dictionary;
    FIXMessage message(raw);

    std::cout << "BodyLength/CheckSum valid: "
              << (message.validate() ? "yes" : "no") << "\n";
    std::cout << "Dictionary valid:          "
              << (dictionary.validate(message) ? "yes" : "no") << "\n\n";

    for (size_t i = 0; i < message.getFieldCount(); i++) {
        string tag = message.getTagAtIndex(i);
        string value = message.getValueAtIndex(i);
        string description = dictionary.getEnumDescription(tag, value);

        std::cout << std::left << std::setw(4) << tag << std::setw(14)
                  << dictionary.getFieldName(tag) << value;
        if (!description.empty()) {
            std::cout << " (" << description << ")";
        }
        std::cout << "\n";
    }

    return 0;
}
