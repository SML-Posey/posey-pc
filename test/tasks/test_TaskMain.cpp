#include "catch2/catch.hpp"
#include "fmt/format.h"

#include "test_common.hpp"
#include "tasks/TaskMain.hpp"
#include "MessageAck.hpp"

TEST_CASE("Test TaskMainTelemetry", "[TaskMain]")
{
    TaskMainTelemetry telem;
    telem.counter = 12;
    telem.t_start = 234;
    telem.t_end = 345;
    telem.invalid_checksum = 6;
    telem.missed_deadline = 7;

    TaskMainTelemetry::Buffer bs;
    telem.serialize(bs);

    bs.rewind();
    REQUIRE(hex(bs.get_buffer()[2]) == "0xc8");
    REQUIRE(bs.used() == TaskMainTelemetry::MessageSize + 3);
    REQUIRE(bs.valid_checksum());
    REQUIRE(hex(bs.get_checksum()) == "0x43");

    TaskMainTelemetry telem2;
    telem2.deserialize(bs);

    REQUIRE(telem2.counter == telem.counter);
    REQUIRE(telem2.t_start == telem.t_start);
    REQUIRE(telem2.t_end == telem.t_end);
    REQUIRE(telem2.invalid_checksum == telem.invalid_checksum);
    REQUIRE(telem2.missed_deadline == telem.missed_deadline);
}
