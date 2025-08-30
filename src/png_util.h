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
        std::vector<std::vector<ubyte>> rawData{};
        size_t scanlineLength{};
        int bitPerRow{};
        int bytesPerRow{};
        int bytesPerPixel{};
        int sections = 0;

        size_t reference{};
        
        unsigned width{};
        unsigned height{};

        size_t getIndex(int x, int y) const;

        pnglib::IHDR ihdr = pnglib::IHDR();
        pnglib::PLTE plte = pnglib::PLTE();
        pnglib::IDAT idat = pnglib::IDAT();
        
    public:

        bool loadfile(const std::string& filename);
        bool savefile(const std::string& filename);

        void InitBPP(std::shared_ptr<pnglib::IHDR>);
        void InitSections(std::shared_ptr<pnglib::IDAT>);

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
        int IHDR(std::shared_ptr<pnglib::IHDR>); 
        int PLTE(std::shared_ptr<pnglib::PLTE>); 
        int tRNS(std::shared_ptr<pnglib::tRNS>);
        int IDAT(std::shared_ptr<pnglib::IDAT>);
        int IEND();

        //Misc
        int Decompress(std::shared_ptr<pnglib::IDAT>);
        int SCANLINE_FORMAT(std::shared_ptr<pnglib::IHDR>);
        int DcompressSize(std::shared_ptr<pnglib::IDAT>, std::shared_ptr<pnglib::IHDR>);
        int scanline(std::shared_ptr<pnglib::IHDR>, std::shared_ptr<pnglib::IDAT>);

        
        // Filter Functions
        int ApplyFilters(std::shared_ptr<pnglib::IDAT> idat);
        int Filter_One(std::shared_ptr<pnglib::IDAT> idat, int);
        int Filter_Two(std::shared_ptr<pnglib::IDAT> idat, int);
        int Filter_Three(std::shared_ptr<pnglib::IDAT> idat, int);
        int Filter_Four(std::shared_ptr<pnglib::IDAT> idat, int);
};

#endif //PNG_H
