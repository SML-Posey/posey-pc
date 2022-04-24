#include "catch2/catch.hpp"

#include "algorithm/ElemRingBuffer.hpp"

TEST_CASE("Test ElemRingBuffer class", "[ElemRingBuffer]")
{
    ElemRingBuffer<int, 3> buf;

    int elem;

    int u, f, c;
    bool da;
    unsigned long d;
    auto cu = [&](int uu, int ff, int cc, bool dada, unsigned long dd) {
        u = uu;
        f = ff;
        c = cc;
        da = dada;
        d = dd;
    };

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next(elem) == false );
    buf.read_latest(); // Always passes, may be junk.

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    buf.write(1);

    cu(1, 2, 3, true, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next(elem) == true );
    REQUIRE( elem == 1 );

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next(elem) == false );

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    // Write one through buffer + commit.
    int & elemr = buf.get_write_buffer();
    elemr = 6;

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next(elem) == false );
    buf.commit_write();

    cu(1, 2, 3, true, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next(elem) == true );
    REQUIRE( elem == 6 );

    cu(0, 3, 3, false, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    // Write 3 indirect.
    buf.write(1);
    buf.write(2);
    buf.write(3);
    cu(3, 0, 3, true, 0);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    // Too many.
    buf.write(4);
    buf.write(5);
    cu(3, 0, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_latest() == 5 );
    cu(3, 0, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next() == 1 );
    cu(2, 1, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next() == 2 );
    cu(1, 2, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next() == 5 );
    cu(0, 3, 3, false, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    REQUIRE( buf.read_next() == 5 );
    cu(0, 3, 3, false, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    // Write 3 more.
    buf.write(1);
    cu(1, 2, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    buf.write(2);
    cu(2, 1, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    buf.write(3);
    cu(3, 0, 3, true, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );

    buf.clear();
    cu(0, 3, 3, false, 2);
    REQUIRE( buf.used() == u );
    REQUIRE( buf.free() == f );
    REQUIRE( buf.capacity() == c );
    REQUIRE( buf.data_available() == da );
    REQUIRE( buf.num_dropped() == d );
}
