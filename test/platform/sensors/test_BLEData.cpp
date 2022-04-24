#include "catch2/catch.hpp"
#include "fmt/format.h"

#include "test_common.hpp"
#include "platform/sensors/BLEData.hpp"

TEST_CASE("Test BLEData class", "[BLEData]")
{
    BLEData data;

    data.time = 55;
    REQUIRE(data.time == 55);

    data.addr[0] = 1;
    data.addr[1] = 2;
    data.addr[2] = 3;
    data.addr[3] = 4;
    data.addr[4] = 5;
    data.addr[5] = 6;
    data.rssi = 10;

    BLEData::Buffer bs;
    data.serialize(bs);
    REQUIRE(hex(bs.get_buffer()[2]) == "0x02");
    REQUIRE(bs.used() == BLEData::MessageSize + 3);
    REQUIRE(bs.valid_checksum());
    REQUIRE(hex(bs.get_checksum()) == "0x0c");

    BLEData data2;
    data2.deserialize(bs);

    REQUIRE(data2.time == data.time);

    REQUIRE(data2.addr[0] == data.addr[0]);
    REQUIRE(data2.addr[1] == data.addr[1]);
    REQUIRE(data2.addr[2] == data.addr[2]);
    REQUIRE(data2.addr[3] == data.addr[3]);
    REQUIRE(data2.addr[4] == data.addr[4]);
    REQUIRE(data2.addr[5] == data.addr[5]);
    REQUIRE(data2.rssi == data.rssi);
}
