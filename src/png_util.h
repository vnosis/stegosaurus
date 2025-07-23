/*
    Written by Matthew Faust

*/

#pragma once
#ifndef PNG_H
#define PNG_H

//Libraries
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

#include "globals/png_global.h"

#define chunkSize 13


class png_util {
    private:
        std::vector<ubyte> fileBuffer{};

        size_t reference = 0;
        
        unsigned width{};
        unsigned height{};

        size_t getIndex(int x, int y) const;
        
    public:

        bool loadfile(const std::string& filename);
        bool savefile(const std::string& filename);

        void fillMap();

        std::vector<std::string> getPngDetails();

        //check if it is png
        bool checkfile();

        bool decode();
        bool encode();
       
        bool embedMessage(const std::string& message);
        std::string extractMessage();
        void clearMessage();

        // Chunks
        void IHDR(std::shared_ptr<pnglib::Chunk_IHDR>); // 13 bytes
        int PLTE(); 
        void IDAT();
        void IEND();


        // Template function
        // Fill out png chunk data 
        // template <typename ChunkType> 
        // void Chunk(ChunkType& chunkArg)
        // {
        //     size_t size = sizeof(ChunkType);    
        //     uint8_t csize = 0;  
        //         for(int i = 0; i < 8; i++) {
        //             std::cout << &fileBuffer[reference] << "file ref" << std::endl;
        //             std::cout << reference << " reference " << std::endl;
        //             reference++;
        //         }

        // };
};


// template <Chunk_IHDR()typename ChunkType>
// class Chunk 
// {
//     private:
//         size_t chunk_sizel = 0;
    
//     public:
//         Chunk();
//         void fillData();
// };



#endif //PNG_H
