#pragma once

#include <fmt/format.h>
#include <stdint.h>

#include "platform/io/BufferSerializer.hpp"

void print_buffer(const uint8_t* const buffer, const uint16_t size) {
    for (auto i = 0; i < size; ++i)
        fmt::print("{:02x}", buffer[i]);
    fmt::print("\n");
}

template <uint16_t N>
void print_buffer(const BufferSerializer<N>& buffer, uint16_t force_size = 0) {
    if (force_size == 0)
        force_size = buffer.used();
    print_buffer(buffer.get_buffer(), force_size);
}
