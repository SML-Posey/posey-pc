#include "catch2/catch.hpp"
#include "test_common.hpp"

#include "platform/io/MessageListener.hpp"
#include "platform/sensors/IMUData.hpp"

TEST_CASE("Test MessageListener class", "[MessageListener]") {
    BufferSerializer<1> m0_src, m0_dst;
    BufferSerializer<2> m1_src, m1_dst;
    BufferSerializer<3> m2_src, m2_dst;
    BufferSerializer<4> m3_src, m3_dst;
    ;

    MessageListener<3, 10> ml;
    REQUIRE(ml.add_listener(0, m0_dst.capacity(), &m0_dst.copy_callback));
    REQUIRE(ml.add_listener(1, m1_dst.capacity(), &m1_dst.copy_callback));
    REQUIRE(ml.add_listener(2, m2_dst.capacity(), &m2_dst.copy_callback));

    REQUIRE(!ml.add_listener(3, m3_dst.capacity(), &m3_dst.copy_callback));

    SECTION("Write/read M0-M2, ignore M3") {
        REQUIRE(ml.process_next() == -1);

        m0_src.write_syncword();
        m0_src.write<uint8_t>(0);
        m0_src.write_checksum();
        ml.write(m0_src.get_buffer(), m0_src.used());

        REQUIRE(ml.process_next() == 0);
        REQUIRE(ml.num_messages() == 1);
        REQUIRE(m0_dst.valid_checksum());
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[0]));
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[1]));
        REQUIRE(m0_dst.read<uint8_t>() == 0);

        m1_src.write_syncword();
        m1_src.write<uint8_t>(1);
        m1_src.write<int8_t>(-12);
        m1_src.write_checksum();
        ml.write(m1_src.get_buffer(), m1_src.used());

        REQUIRE(ml.process_next() == 1);
        REQUIRE(ml.num_messages() == 2);
        REQUIRE(m1_dst.valid_checksum());
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[0]));
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[1]));
        REQUIRE(m1_dst.read<uint8_t>() == 1);
        REQUIRE(m1_dst.read<int8_t>() == -12);

        REQUIRE(ml.process_next() == -1);

        // Buffer wraps this time.

        m2_src.write_syncword();
        m2_src.write<uint8_t>(2);
        m2_src.write<int16_t>(-923);
        m2_src.write_checksum();
        ml.write(m2_src.get_buffer(), m2_src.used());

        REQUIRE(ml.process_next() == 2);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(m2_dst.valid_checksum());
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[0]));
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[1]));
        REQUIRE(m2_dst.read<uint8_t>() == 2);
        REQUIRE(m2_dst.read<int16_t>() == -923);

        REQUIRE(ml.process_next() == -1);

        m3_src.write_syncword();
        m3_src.write<uint8_t>(3);
        m3_src.write<uint16_t>(8832);
        m3_src.write<uint8_t>(23);
        m3_src.write_checksum();
        ml.write(m3_src.get_buffer(), m3_src.used());

        REQUIRE(ml.process_next() == -1);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(ml.num_ignored_messages() == 1);
    }

    SECTION("With Junk") {
        constexpr uint8_t JunkSize = 3;
        uint8_t junk[JunkSize] = {0x12, 0x34, 0x56};

        REQUIRE(ml.process_next() == -1);

        REQUIRE(ml.write(junk, JunkSize) == JunkSize);
        REQUIRE(ml.process_next() == -1);
        // No bytes skipped. Need at least 4 to consider the buffer.
        REQUIRE(ml.num_skipped_bytes() == 0);
        REQUIRE(ml.write(junk, JunkSize) == JunkSize);
        REQUIRE(ml.process_next() == -1);
        // We'll skip 3 before we stop checking.
        REQUIRE(ml.num_skipped_bytes() == 3);

        REQUIRE(ml.write(junk, JunkSize) == JunkSize);
        m0_src.write_syncword();
        m0_src.write<uint8_t>(0);
        m0_src.write_checksum();
        REQUIRE(ml.write(m0_src.get_buffer(), m0_src.used()) == m0_src.used());

        // Too many bytes.
        REQUIRE(ml.write(junk, JunkSize) == 0);

        REQUIRE(ml.process_next() == 0);
        REQUIRE(ml.num_messages() == 1);
        REQUIRE(ml.num_skipped_bytes() == JunkSize * 3);
        REQUIRE(ml.write(junk, JunkSize) == 3);

        REQUIRE(m0_dst.valid_checksum());
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[0]));
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[1]));
        REQUIRE(m0_dst.read<uint8_t>() == 0);

        m1_src.write_syncword();
        m1_src.write<uint8_t>(1);
        m1_src.write<int8_t>(-12);
        m1_src.write_checksum();
        REQUIRE(ml.write(m1_src.get_buffer(), m1_src.used()) == m1_src.used());

        REQUIRE(ml.process_next() == 1);
        REQUIRE(ml.num_skipped_bytes() == JunkSize * 4);
        REQUIRE(ml.num_messages() == 2);

        REQUIRE(m1_dst.valid_checksum());
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[0]));
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[1]));
        REQUIRE(m1_dst.read<uint8_t>() == 1);
        REQUIRE(m1_dst.read<int8_t>() == -12);

        REQUIRE(ml.process_next() == -1);

        m2_src.write_syncword();
        m2_src.write<uint8_t>(2);
        m2_src.write<int16_t>(-923);
        m2_src.write_checksum();
        REQUIRE(ml.write(m2_src.get_buffer(), m2_src.used()) == m2_src.used());

        REQUIRE(ml.process_next() == 2);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(m2_dst.valid_checksum());
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[0]));
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[1]));
        REQUIRE(m2_dst.read<uint8_t>() == 2);
        REQUIRE(m2_dst.read<int16_t>() == -923);

        REQUIRE(ml.process_next() == -1);

        m3_src.write_syncword();
        m3_src.write<uint8_t>(3);
        m3_src.write<uint16_t>(8832);
        m3_src.write<uint8_t>(23);
        m3_src.write_checksum();
        REQUIRE(ml.write(m3_src.get_buffer(), m3_src.used()) == m3_src.used());

        REQUIRE(ml.process_next() == -1);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(ml.num_ignored_messages() == 1);
    }

    SECTION("Via polling source") {
        class PollWrapper : public BaseMessageReader {
            public:
                PollWrapper(const uint8_t* buffer, const uint16_t size)
                    : buffer(buffer), bytes_available(size) {}

                uint16_t read_to(uint8_t* dst, uint16_t size) override {
                    if (size > bytes_available)
                        size = bytes_available;
                    for (auto di = 0; di < size; ++di)
                        dst[di] = buffer[di];
                    bytes_available -= size;
                    buffer += size;
                    return size;
                }

            private:
                const uint8_t* buffer;
                uint16_t bytes_available;
        };
        REQUIRE(ml.process_next() == -1);

        m0_src.write_syncword();
        m0_src.write<uint8_t>(0);
        m0_src.write_checksum();
        REQUIRE(
            ml.poll(PollWrapper(m0_src.get_buffer(), m0_src.used())) ==
            m0_src.used());

        REQUIRE(ml.process_next() == 0);
        REQUIRE(ml.num_messages() == 1);
        REQUIRE(m0_dst.valid_checksum());
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[0]));
        REQUIRE(hex(m0_dst.read<uint8_t>()) == hex(m0_dst.syncword[1]));
        REQUIRE(m0_dst.read<uint8_t>() == 0);

        m1_src.write_syncword();
        m1_src.write<uint8_t>(1);
        m1_src.write<int8_t>(-12);
        m1_src.write_checksum();
        REQUIRE(
            ml.poll(PollWrapper(m1_src.get_buffer(), m1_src.used())) ==
            m1_src.used());

        REQUIRE(ml.process_next() == 1);
        REQUIRE(ml.num_messages() == 2);
        REQUIRE(m1_dst.valid_checksum());
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[0]));
        REQUIRE(hex(m1_dst.read<uint8_t>()) == hex(m1_dst.syncword[1]));
        REQUIRE(m1_dst.read<uint8_t>() == 1);
        REQUIRE(m1_dst.read<int8_t>() == -12);

        REQUIRE(ml.process_next() == -1);

        // Buffer wraps this time.

        m2_src.write_syncword();
        m2_src.write<uint8_t>(2);
        m2_src.write<int16_t>(-923);
        m2_src.write_checksum();
        REQUIRE(
            ml.poll(PollWrapper(m2_src.get_buffer(), m2_src.used())) ==
            m2_src.used());

        REQUIRE(ml.process_next() == 2);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(m2_dst.valid_checksum());
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[0]));
        REQUIRE(hex(m2_dst.read<uint8_t>()) == hex(m2_dst.syncword[1]));
        REQUIRE(m2_dst.read<uint8_t>() == 2);
        REQUIRE(m2_dst.read<int16_t>() == -923);

        REQUIRE(ml.process_next() == -1);

        m3_src.write_syncword();
        m3_src.write<uint8_t>(3);
        m3_src.write<uint16_t>(8832);
        m3_src.write<uint8_t>(23);
        m3_src.write_checksum();
        REQUIRE(
            ml.poll(PollWrapper(m3_src.get_buffer(), m3_src.used())) ==
            m3_src.used());

        REQUIRE(ml.process_next() == -1);
        REQUIRE(ml.num_messages() == 3);
        REQUIRE(ml.num_ignored_messages() == 1);
    }

    SECTION("With class") {
        IMUData src, dst;
        src.time = 10;
        src.An = 1;
        src.Gn = 2;
        src.Mn = 3;
        src.Qn = 4;
        src.Ax = 11;
        src.Ay = 12;
        src.Az = 13;
        src.Gx = 21;
        src.Gy = 22;
        src.Gz = 23;
        src.Mx = 31;
        src.My = 32;
        src.Mz = 33;
        src.Qi = 41;
        src.Qj = 42;
        src.Qk = 43;
        src.Qr = 44;
        src.Qacc = 45;

        IMUData::Buffer buf_src, buf_dst;
        src.serialize(buf_src);

        MessageListener<1, 100> ml2;
        ml2.add_listener(
            src.message_id, buf_src.capacity(), &buf_dst.copy_callback);
        ml2.write(buf_src.get_buffer(), buf_src.used());
        REQUIRE(ml2.process_next() == IMUData::message_id);

        dst.deserialize(buf_dst);
        REQUIRE(dst.time == src.time);
        REQUIRE(dst.Ax == src.Ax);
        REQUIRE(dst.Ay == src.Ay);
        REQUIRE(dst.Az == src.Az);
        REQUIRE(dst.Gx == src.Gx);
        REQUIRE(dst.Gy == src.Gy);
        REQUIRE(dst.Gz == src.Gz);
        REQUIRE(dst.Mx == src.Mx);
        REQUIRE(dst.My == src.My);
        REQUIRE(dst.Mz == src.Mz);
    }
}
