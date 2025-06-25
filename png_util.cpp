#include "png_util.h"
#include <fstream> 
#include <iostream>




size_t png_util::getIndex(int x, int y) const
{
    return size_t();
}

bool png_util::loadfile(std::string &filename)
{
    std::ifstream pngFile(filename, std::ios::binary);
    if(!pngFile) {
        return false;
    }

    this->fileBuffer = std::vector<byte>((std::istreambuf_iterator<char>(pngFile)),{});
    
    return false;
}

bool png_util::savefile(const std::string &filename)
{
    std::ofstream pngFile(filename, std::ios::binary);
    if(!filename) 
    {
        return false;
    }

    pngFile.write(reinterpret_cast<const char*>(this->fileBuffer.data()), fileBuffer.size());
    return false;
}

bool png_util::decode()
{
    // Todo :  parse PNG, decompress IDAT etc, populate pixels
    return false;
}

bool png_util::encode()
{

    //Todo: Parse chunks, generate filebuffer
    return false;
}

bool png_util::embedMessage(const std::string &message)
{
    return false;
}

std::string png_util::extractMessage()
{
    return std::string();
}

void png_util::clearMessage()
{
}
