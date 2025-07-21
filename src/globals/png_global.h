#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>
#include <string>


using ubyte4 = uint32_t;  
using ubyte = uint8_t;

namespace pnglib {
    inline constexpr std::array<ubyte, 8> PNG_SIGNATURE {
        0x89,
        0x50,
        0x4E,
        0x47,
        0x0D,
        0x0A,
        0x1A,
        0x0A
    };

    inline constexpr std::array<ubyte, 4> IHDR {
        0x49,
        0x48,
        0x44,
        0x52
    };

    struct Chunk_IHDR {
        ubyte4 width{};
        ubyte4 height{};
        ubyte depth{};
        ubyte color_type{}; 
        ubyte compression{};
        ubyte filter{};
        ubyte interlace{};
    };

    inline std::unordered_map<std::string, bool> ChunkMap{
        {"PNG", false},
        {"IHDR", false},
    }; 
}