#include "png_util.h"
#include <fstream> 
#include <iostream>
#include <unistd.h>
#include <limits.h>




size_t png_util::getIndex(int x, int y) const
{
    return size_t();
}

bool png_util::loadfile(const std::string &filename)
{

    // CWD test
    // std::cout << filename << std::endl;
    // char cwd[PATH_MAX];
    // if(getcwd(cwd, sizeof(cwd)) != NULL) {
    //     std::cout << cwd << std::endl;
    // }

    std::ifstream pngFile(filename, std::ios_base::binary);
    if(!pngFile) {
        std::cerr << "filed to open";
        return false;
    }

    this->fileBuffer = std::vector<byte>((std::istreambuf_iterator<char>(pngFile)),{});
    std::cout << fileBuffer.size() << " size " << std::endl;
    return true;
}

bool png_util::savefile(const std::string &filename)
{
    std::ofstream binaryFile(filename, std::ios::binary);
    if(!binaryFile) 
    {
        return false;
    }

    binaryFile.write(reinterpret_cast<const char*>(this->fileBuffer.data()), fileBuffer.size());
    return true;
}

bool png_util::checkfile() 
{
    size_t i = 0;
    if(this->fileBuffer.size() < pnglib::PNG_SIGNATURE.size()) return false;

    for(auto& hexSignature : pnglib::PNG_SIGNATURE) 
    {

        if(hexSignature != this->fileBuffer[i++])
        {
            return false;
        }
    }

    return true;
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
