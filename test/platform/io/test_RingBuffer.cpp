#include "catch2/catch.hpp"
#include "test_common.hpp"

#include "algorithm"/RingBuffer.h"

// Test RingBuffer.
TEST_CASE("RingBuffer", "[RingBuffer]")
{
    // Test write_from().
    SECTION("write_from and read_to")
    {
        RingBuffer<uint8_t, 4> rb;
        REQUIRE(rb.capacity() == 4);
        REQUIRE(rb.used() == 0);
        REQUIRE(rb.free() == 4);

        uint8_t buffer[4] = { 0, 1, 2, 3 };
        REQUIRE(rb.write_from(buffer, 4) == 4);
        REQUIRE(rb.used() == 4);
        REQUIRE(rb.free() == 0);

        REQUIRE(rb.write_from(buffer, 4) == 0);
        REQUIRE(rb.used() == 4);
        REQUIRE(rb.free() == 0);

        auto buffer = rb.get_buffer();
        REQUIRE(buffer[0] == 0);
        REQUIRE(buffer[1] == 1);
        REQUIRE(buffer[2] == 2);
        REQUIRE(buffer[3] == 3);

        uint8_t read_buffer[4];
        REQUIRE(rb.read_to(read_buffer, 4) == 4);
        REQUIRE(rb.used() == 0);
        REQUIRE(rb.free() == 4);

        REQUIRE(read_buffer[0] == 0);
        REQUIRE(read_buffer[1] == 1);
        REQUIRE(read_buffer[2] == 2);
        REQUIRE(read_buffer[3] == 3);

        REQUIRE(rb.read_to(read_buffer, 4) == 0);
        REQUIRE(rb.used() == 0);
        REQUIRE(rb.free() == 4);
    }

    // Test circular writing and reading.
    SECTION("Test wrapping")
    {
        RingBuffer<uint8_t, 4> rb;
        uint8_t buffer[4] = { 0, 1, 2, 3 };
        REQUIRE(rb.write_from(buffer, 2) == 2);
        REQUIRE(rb.used() == 2);
        REQUIRE(rb.free() == 2);

        uint8_t read_buffer[4];
        REQUIRE(rb.read_to(read_buffer, 1) == 1);
        REQUIRE(rb.used() == 1);
        REQUIRE(rb.free() == 3);
        REQUIRE(read_buffer[0] == 0);

        REQUIRE(rb.write_from(buffer, 4) == 3);
        REQUIRE(rb.used() == 4);
        REQUIRE(rb.free() == 0);

        auto rb_buffer = rb.get_buffer();
        REQUIRE(rb_buffer[0] == 2);
        REQUIRE(rb_buffer[1] == 1);
        REQUIRE(rb_buffer[2] == 0);
        REQUIRE(rb_buffer[3] == 1);

        REQUIRE(rb.read_to(read_buffer, 4) == 4);
        REQUIRE(rb.used() == 0);
        REQUIRE(rb.free() == 4);
        REQUIRE(read_buffer[0] == 1);
        REQUIRE(read_buffer[1] == 0);
        REQUIRE(read_buffer[2] == 1);
        REQUIRE(read_buffer[3] == 2);        
    }
}