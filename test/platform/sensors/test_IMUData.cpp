#include "catch2/catch.hpp"
#include "fmt/format.h"

#include "platform/sensors/IMUData.hpp"
#include "test_common.hpp"

TEST_CASE("Test IMUData class", "[IMUData]") {
    IMUData data;

    data.time = 55;
    REQUIRE(data.time == 55);

    data.An = 1;
    data.Gn = 2;
    data.Mn = 3;
    data.Qn = 4;

    data.Ax = 11;
    data.Ay = 12;
    data.Az = 13;

    data.Gx = 21;
    data.Gy = 22;
    data.Gz = 23;

    data.Mx = 31;
    data.My = 32;
    data.Mz = 33;

    data.Qi = 41;
    data.Qj = 42;
    data.Qk = 43;
    data.Qr = 44;
    data.Qacc = 45;

    IMUData::Buffer bs;
    data.serialize(bs);
    REQUIRE(hex(bs.get_buffer()[2]) == "0x01");
    REQUIRE(bs.used() == IMUData::MessageSize + 3);
    REQUIRE(bs.valid_checksum());
    REQUIRE(hex(bs.get_checksum()) == "0x5d");

    IMUData data2;
    data2.deserialize(bs);

    REQUIRE(data2.time == data.time);

    REQUIRE(data2.An == data.An);
    REQUIRE(data2.Gn == data.Gn);
    REQUIRE(data2.Mn == data.Mn);
    REQUIRE(data2.Qn == data.Qn);

    REQUIRE(data2.Ax == data.Ax);
    REQUIRE(data2.Ay == data.Ay);
    REQUIRE(data2.Az == data.Az);

    REQUIRE(data2.Gx == data.Gx);
    REQUIRE(data2.Gy == data.Gy);
    REQUIRE(data2.Gz == data.Gz);

    REQUIRE(data2.Mx == data.Mx);
    REQUIRE(data2.My == data.My);
    REQUIRE(data2.Mz == data.Mz);

    REQUIRE(data2.Qi == data.Qi);
    REQUIRE(data2.Qj == data.Qj);
    REQUIRE(data2.Qk == data.Qk);
    REQUIRE(data2.Qr == data.Qr);
    REQUIRE(data2.Qacc == data.Qacc);
}
