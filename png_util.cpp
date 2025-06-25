#include "png_util.h"
#include <fstream> 




size_t png_util::getIndex(int x, int y) const
{
    return size_t();
}

bool png_util::loadfile(std::string &filename)
{
    std::ifstream pngImage(filename, std::ios::binary);
    
    return false;
}

bool png_util::savefile(std::string &filename)
{
    return false;
}

bool png_util::decode()
{
    return false;
}

bool png_util::encode()
{
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
