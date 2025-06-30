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
#include "globals/png_global.h"

#define chunkSize 13




class png_util {
    private:
        std::vector<byte> fileBuffer{};
        
        unsigned width{};
        unsigned height{};

        size_t getIndex(int x, int y) const;

    public:

        bool loadfile(const std::string& filename);
        bool savefile(const std::string& filename);

        std::vector<std::string> getPngDetails();

        //check if it is png
        bool checkfile();

        bool decode();
        bool encode();
       
        bool embedMessage(const std::string& message);
        std::string extractMessage();
        void clearMessage();
};


template <typename T, int N>
class Chunk 
{
    T elem[N];
    
}



#endif //PNG_H
