#pragma once

#include <cstdint>
#include <array>


using byte = uint8_t;

namespace pnglib {
    inline constexpr std::array<byte, 8> PNG_SIGNATURE {
        0x89,
        0x50,
        0x4E,
        0x47,
        0x0D,
        0x0A,
        0x1A,
        0x0A
    };

    inline constexpr std::array<byte, 4> IHDR {
        0x49,
        0x48,
        0x44,
        0x52
    };
}