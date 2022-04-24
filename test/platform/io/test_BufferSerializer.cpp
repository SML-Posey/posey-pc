#include "catch2/catch.hpp"
#include "test_common.hpp"

#include "platform/io/BufferSerializer.hpp"

TEST_CASE("Test BufferSerializer class", "[BufferSerializer]")
{
    struct Data
    {
        uint8_t u8 = 0;
        uint16_t u16 = 0;
        uint32_t u32 = 0;
        int8_t i8 = 0;
        int16_t i16 = 0;
        int32_t i32 = 0;
        float f32 = 0;
        double f64 = 0;

        Data(
            uint8_t u8 = 0,
            uint16_t u16 = 0,
            uint32_t u32 = 0,
            int8_t i8 = 0,
            int16_t i16 = 0,
            int32_t i32 = 0,
            float f32 = 0,
            double f64 = 0) :
            u8(u8), u16(u16), u32(u32),
            i8(i8), i16(i16), i32(i32),
            f32(f32), f64(f64) {}
    };

    Data data(
        0x12, 0x3456, 0x78910123,
        0x45, 0x1234, 0x98765432,
        3.123456789101112,
        3.123456789101112
    );

    constexpr uint16_t DataSize = 26;

    BufferSerializer<DataSize> bs;
    REQUIRE(bs.capacity() == DataSize + 3);
    REQUIRE(bs.used() == 0);

    bs.write_syncword();
    REQUIRE(bs.used() == 2);

    bs.write(data.u8);
    REQUIRE(bs.used() == 3);

    bs.write(data.u16);
    REQUIRE(bs.used() == 5);

    bs.write(data.u32);
    REQUIRE(bs.used() == 9);

    bs.write(data.i8);
    REQUIRE(bs.used() == 10);

    bs.write(data.i16);
    REQUIRE(bs.used() == 12);

    bs.write(data.i32);
    REQUIRE(bs.used() == 16);

    bs.write(data.f32);
    REQUIRE(bs.used() == 20);

    bs.write(data.f64);
    REQUIRE(bs.used() == 28);

    bs.write_checksum();
    REQUIRE(bs.used() == 29);

    REQUIRE(bs.get_checksum() == 0x3F);

    bs.rewind();
    REQUIRE(bs.valid_checksum());

    REQUIRE(bs.read<uint8_t>() == bs.syncword[0]);
    REQUIRE(bs.read<uint8_t>() == bs.syncword[1]);
    REQUIRE(bs.read<uint8_t>() == data.u8);
    REQUIRE(bs.read<uint16_t>() == data.u16);
    REQUIRE(bs.read<uint32_t>() == data.u32);
    REQUIRE(bs.read<int8_t>() == data.i8);
    REQUIRE(bs.read<int16_t>() == data.i16);
    REQUIRE(bs.read<int32_t>() == data.i32);
    REQUIRE(bs.read<float>() == data.f32);
    REQUIRE(bs.read<double>() == data.f64);
    REQUIRE(bs.read<uint8_t>() == bs.get_checksum());

    SECTION("Test copy callback")
    {
        BufferSerializer<DataSize> bs2;
        bs2.copy_callback.reset();
        bs2.copy_callback.copy(bs.get_buffer());

        bs2.rewind();
        REQUIRE(bs2.valid_checksum());

        REQUIRE(bs2.read<uint8_t>() == bs.syncword[0]);
        REQUIRE(bs2.read<uint8_t>() == bs.syncword[1]);
        REQUIRE(bs2.read<uint8_t>() == data.u8);
        REQUIRE(bs2.read<uint16_t>() == data.u16);
        REQUIRE(bs2.read<uint32_t>() == data.u32);
        REQUIRE(bs2.read<int8_t>() == data.i8);
        REQUIRE(bs2.read<int16_t>() == data.i16);
        REQUIRE(bs2.read<int32_t>() == data.i32);
        REQUIRE(bs2.read<float>() == data.f32);
        REQUIRE(bs2.read<double>() == data.f64);
        REQUIRE(bs2.read<uint8_t>() == bs2.get_checksum());
    }
}
