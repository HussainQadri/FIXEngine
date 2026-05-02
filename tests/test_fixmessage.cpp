#include "FIXMessage.h"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>

std::string withSoh(std::string s) {
    std::replace(s.begin(), s.end(), '|', '\x01');
    return s;
}

TEST_CASE("FIXMessage validates correctly on empty string",
          "[fix][parse][empty]") {
    std::string raw = withSoh("");
    FIXMessage msg(raw);
    REQUIRE(msg.getFieldCount() == 0);
    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage parses a valid heartbeat", "[fix][parse][heartbeat]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=55|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=065|");

    FIXMessage msg(raw);

    REQUIRE(msg.getFieldCount() == 8);
    CHECK(msg.getTagAtIndex(0) == "8");
    CHECK(msg.getValueAtIndex(0) == "FIX.4.4");
    CHECK(msg.getTagAtIndex(1) == "9");
    CHECK(msg.getValueAtIndex(1) == "55");
    CHECK(msg.getTagAtIndex(2) == "35");
    CHECK(msg.getValueAtIndex(2) == "0");
    CHECK(msg.getTagAtIndex(7) == "10");
    CHECK(msg.getValueAtIndex(7) == "065");
    CHECK(msg.validate());
}

TEST_CASE("FIXMessage preserves field order", "[fix][parse][order]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=73|"
                              "35=A|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=1|"
                              "52=20260311-12:00:00.000|"
                              "98=0|"
                              "108=30|"
                              "141=Y|"
                              "10=151|");

    FIXMessage msg(raw);

    REQUIRE(msg.getFieldCount() == 11);
    CHECK(msg.getTagAtIndex(0) == "8");
    CHECK(msg.getTagAtIndex(1) == "9");
    CHECK(msg.getTagAtIndex(2) == "35");
    CHECK(msg.getTagAtIndex(3) == "49");
    CHECK(msg.getTagAtIndex(4) == "56");
    CHECK(msg.getTagAtIndex(5) == "34");
    CHECK(msg.getTagAtIndex(6) == "52");
    CHECK(msg.getTagAtIndex(7) == "98");
    CHECK(msg.getTagAtIndex(8) == "108");
    CHECK(msg.getTagAtIndex(9) == "141");
    CHECK(msg.getTagAtIndex(10) == "10");
    CHECK(msg.validate());
}

TEST_CASE("FIXMessage parses a valid NewOrderSingle",
          "[fix][parse][order-single]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=133|"
                              "35=D|"
                              "49=BUY1|"
                              "56=SELL1|"
                              "34=5|"
                              "52=20260311-12:01:00.000|"
                              "11=ORD1001|"
                              "21=1|"
                              "55=MSFT|"
                              "54=1|"
                              "38=100|"
                              "40=2|"
                              "44=310.25|"
                              "59=0|"
                              "60=20260311-12:01:00.000|"
                              "10=194|");

    FIXMessage msg(raw);

    REQUIRE(msg.getFieldCount() == 17);
    CHECK(msg.getValueAtIndex(2) == "D");
    CHECK(msg.getTagAtIndex(7) == "11");
    CHECK(msg.getValueAtIndex(7) == "ORD1001");
    CHECK(msg.getTagAtIndex(9) == "55");
    CHECK(msg.getValueAtIndex(9) == "MSFT");
    CHECK(msg.getTagAtIndex(13) == "44");
    CHECK(msg.getValueAtIndex(13) == "310.25");
    CHECK(msg.validate());
}

TEST_CASE("FIXMessage parses a valid ExecutionReport",
          "[fix][parse][execution-report]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=185|"
                              "35=8|"
                              "49=SELL1|"
                              "56=BUY1|"
                              "34=6|"
                              "52=20260311-12:01:00.100|"
                              "37=EX12345|"
                              "11=ORD1001|"
                              "17=ER12345|"
                              "20=0|"
                              "39=0|"
                              "150=0|"
                              "55=MSFT|"
                              "54=1|"
                              "38=100|"
                              "40=2|"
                              "44=310.25|"
                              "59=0|"
                              "32=0|"
                              "31=0|"
                              "14=0|"
                              "6=0|"
                              "60=20260311-12:01:00.100|"
                              "10=056|");

    FIXMessage msg(raw);

    REQUIRE(msg.getFieldCount() == 25);
    CHECK(msg.getValueAtIndex(2) == "8");
    CHECK(msg.getTagAtIndex(7) == "37");
    CHECK(msg.getValueAtIndex(7) == "EX12345");
    CHECK(msg.getTagAtIndex(12) == "150");
    CHECK(msg.getValueAtIndex(12) == "0");
    CHECK(msg.validate());
}

TEST_CASE("FIXMessage rejects incorrect checksum",
          "[fix][validate][checksum]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=55|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=999|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage rejects incorrect BodyLength",
          "[fix][validate][bodylength]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=999|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=065|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage rejects missing checksum field",
          "[fix][validate][checksum]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=55|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage rejects non numeric checksum",
          "[fix][validate][checksum]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=55|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=ABC|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage handles empty value field", "[fix][malformed][empty]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=48|"
                              "35=0|"
                              "49=|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=114|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage rejects checksum not in final position",
          "[fix][malformed][checksum]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=62|"
                              "35=0|"
                              "49=SENDER|"
                              "10=000|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage can still expose duplicate tags in parsed order",
          "[fix][parse][duplicate-tags]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=67|"
                              "35=0|"
                              "49=SENDER|"
                              "49=OTHER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=214|");

    FIXMessage msg(raw);

    REQUIRE(msg.getFieldCount() == 9);
    CHECK(msg.getTagAtIndex(3) == "49");
    CHECK(msg.getValueAtIndex(3) == "SENDER");
    CHECK(msg.getTagAtIndex(4) == "49");
    CHECK(msg.getValueAtIndex(4) == "OTHER");
    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage rejects truncated message",
          "[fix][malformed][truncated]") {
    std::string raw = withSoh("8=FIX.4.4|"
                              "9=55|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.");

    FIXMessage msg(raw);

    CHECK_FALSE(msg.validate());
}

TEST_CASE("FIXMessage serialize recalculates BodyLength and CheckSum",
          "[fix][serialize]") {
    std::string raw = withSoh("8=FIX.4.2|"
                              "9=999|"
                              "35=0|"
                              "49=SENDER|"
                              "56=TARGET|"
                              "34=2|"
                              "52=20260311-12:00:00.000|"
                              "10=999|");

    FIXMessage msg(raw);
    std::string serialized = msg.serialize();
    FIXMessage reparsed(serialized);

    CHECK(reparsed.validate());
    CHECK(reparsed.getValue("9") == "55");
    CHECK(reparsed.getValue("10") == "063");
}

TEST_CASE("FIXMessage can build and serialize from empty constructor",
          "[fix][serialize][builder]") {
    FIXMessage msg;
    msg.addField("8", "FIX.4.2");
    msg.addField("35", "0");
    msg.addField("49", "SENDER");
    msg.addField("56", "TARGET");
    msg.addField("34", "2");
    msg.addField("52", "20260311-12:00:00.000");

    std::string serialized = msg.serialize();
    FIXMessage reparsed(serialized);

    CHECK(reparsed.validate());
    CHECK(reparsed.getFieldCount() == 8);
    CHECK(reparsed.getValue("8") == "FIX.4.2");
    CHECK(reparsed.getValue("9") == "55");
    CHECK(reparsed.getValue("35") == "0");
    CHECK(reparsed.getValue("10") == "063");
}
