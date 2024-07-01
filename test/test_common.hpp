#pragma once

#include "fmt/format.h"

inline std::string hex(const uint8_t byte) {
    return fmt::format("0x{:02x}", byte);
}
