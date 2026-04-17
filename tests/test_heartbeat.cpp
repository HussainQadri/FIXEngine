#include "Heartbeat.h"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <string>

static std::string soh(std::string s) {
    std::replace(s.begin(), s.end(), '|', '\x01');
    return s;
}

TEST_CASE("Heartbeat msgType", "[heartbeat]") {
    Heartbeat hb(soh("8=FIX.4.2|9=58|35=0|49=A|56=B|34=1|"
                     "52=20190605-12:19:52|10=000|"));
    CHECK(hb.msgType() == "0");
}

TEST_CASE("Heartbeat getTestReqID present", "[heartbeat]") {
    Heartbeat hb(soh("8=FIX.4.2|9=58|35=0|49=A|56=B|34=1|"
                     "52=20190605-12:19:52|112=REQ123|10=000|"));
    CHECK(hb.getTestReqID() == "REQ123");
}

TEST_CASE("Heartbeat getTestReqID absent", "[heartbeat]") {
    Heartbeat hb(soh("8=FIX.4.2|9=58|35=0|49=A|56=B|34=1|"
                     "52=20190605-12:19:52|10=000|"));
    CHECK(hb.getTestReqID() == "");
}
