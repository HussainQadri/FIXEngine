#include <iostream>
#include <string>

class FIXMessage {
  private:
    std::string symbol;
    float price;
    int quantity;
    std::string side;

    FIXMessage(std::string symbol, float price, int quantity,
               std::string side) {
        this->symbol = symbol;
        this->price = price;
        this->quantity = quantity;
        this->side = side;
    }
};

void Parse(FIXMessage message) {}

int main() { return 0; }
