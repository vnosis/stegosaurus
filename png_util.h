/*
    Written by Matthew Faust

    This is just a personal tool in its infancy
*/

#pragma once
#ifndef PNG_H
#define PNG_H

//Libraries
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using byte = uint8_t;

class png_util {
    private:
        std::vector<byte> fileBuffer{};
        std::vector<byte> pixels{};
        unsigned width{};
        unsigned height{};

        size_t getIndex(int x, int y) const;

    public:

        bool loadfile(std::string& filename);
        bool savefile(const std::string& filename);

        bool decode();
        bool encode();
       
        bool embedMessage(const std::string& message);
        std::string extractMessage();
        void clearMessage();
};

extern png_util globalPngUtil;


#endif //PNG_H
