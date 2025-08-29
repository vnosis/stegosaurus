#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>
#include <string>
#include <queue>


using ubyte4 = uint32_t;  
using ubyte2 = uint16_t;
using ubyte  = uint8_t;

constexpr int ERROR_INVALID_IHDR     = -1;
constexpr int ERROR_INVALID_PLTE     = -2;
constexpr int ERROR_INVALID_IDAT     = -3;
constexpr int ERROR_INVALID_IEND     = -4;
constexpr int ERROR_INVALID_SCANLINE = -5;
constexpr int SUCCESS = 1;


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
    inline std::queue<ubyte> IENDQ({0x49,0x45,0x4e,0x44});
    inline std::queue<ubyte> IDATQ({0x49,0x44,0x41,0x54});

    // inline constexpr std::array<ubyte, 4> IHDR {
    //     0x49,
    //     0x48,
    //     0x44,
    //     0x52
    // };

    struct IHDR {
        ubyte4 ihdrSize{};
        ubyte4 width{};
        ubyte4 height{};
        ubyte depth{};
        ubyte color_type{}; 
        ubyte compression{};
        ubyte filter{};
        ubyte interlace{};
        int scanlineLenght{};
        int byteperpixel{};
    };

    struct tRNS{
    };

    struct PLTE {
        ubyte4 size{};
        std::vector<std::vector<ubyte>> rgbData{};
    };
   
    struct IDAT {
        ubyte4 size{};       
        std::vector<ubyte> compressedD{}; 
        std::vector<ubyte> decompressedD{};
        std::vector<std::vector<ubyte>> filtered_rows{};
        std::vector<ubyte> filterFunctions{};
        std::vector<std::vector<ubyte>> rawdata{};
    };
    // In case of multiple IDAT's
    // std::vector<IDAT> IDATS{};

    inline std::unordered_map<std::string, bool> ChunkMap{
        {"PNG", false},
        {"IHDR", false},
        {"PLTE", false},
        {"IDAT", false},
        {"IEND", false},
    }; 

    // Find a better way to represent this
    inline std::unordered_map<int, int> ScanSample{
        {0,1},
        {2,3},
        {3,1},
        {4,2},
        {6,4},
    };
}