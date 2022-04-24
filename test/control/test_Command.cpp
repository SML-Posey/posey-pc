#include "catch2/catch.hpp"
#include "fmt/format.h"

#include "test_common.hpp"
#include "control/Command.hpp"
#include "MessageAck.hpp"

TEST_CASE("Test command", "[Command]")
{
    Command cmd;
    cmd.command = Command::NoOp;
    cmd.arg1 = 10;
    cmd.arg2 = 11;
    cmd.arg3 = 12;
    cmd.ack = MessageAck::Expected;

    Command::Buffer bs;
    cmd.serialize(bs);

    bs.rewind();
    REQUIRE(hex(bs.get_buffer()[2]) == "0x28");
    REQUIRE(bs.used() == Command::MessageSize + 3);
    REQUIRE(bs.valid_checksum());
    REQUIRE(hex(bs.get_checksum()) == "0xa4");

    Command cmd2;
    cmd2.deserialize(bs);

    REQUIRE(cmd2.command == cmd.command);
    REQUIRE(cmd2.arg1 == cmd.arg1);
    REQUIRE(cmd2.arg2 == cmd.arg2);
    REQUIRE(cmd2.arg3 == cmd.arg3);
    REQUIRE(cmd2.ack == cmd.ack);
}
