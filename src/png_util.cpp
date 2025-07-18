#include "png_util.h"
#include <fstream> 
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <queue>




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

    this->fileBuffer = std::vector<ubyte>((std::istreambuf_iterator<char>(pngFile)),{});
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

std::vector<std::string> getPngDetails() 
{
    return std::vector<std::string>();
};

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
        reference++;
    }

    pnglib::ChunkMap["PNG"] = true;
    std::cout << pnglib::ChunkMap["PNG"] << std::endl;

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

void png_util::IHDR(std::shared_ptr<pnglib::Chunk_IHDR> ihdr) 
{

    std::queue<ubyte> ihdrQ({0x49,0x48,0x44,0x52});
    std::queue<ubyte> fileQ{};

    int ihdrSize = 4;

    for(int i = 0; i < ihdrSize; i++) 
    {
        fileQ.push(this->fileBuffer[reference]);
    }

    while(reference < this->fileBuffer.size() && fileQ != ihdrQ) 
    {
        fileQ.pop();
        fileQ.push(this->fileBuffer[reference++]);
    }
     

    if(reference < this->fileBuffer.size())
    {
        std::cout << "IHDR chunk found" << std::endl;
        pnglib::ChunkMap["IHDR"] = true;
    }

    // Fill width and height
    uint8_t index = 0;
    std::string width{};
    std::cout << this->fileBuffer[reference] << std::endl;
    while(reference < this->fileBuffer.size() && index < 4) {
        ihdr->width <<= 8;
        ihdr->width |= this->fileBuffer[reference++];
        index++;
    }
    index = 0;
    while(reference < this->fileBuffer.size() && index < 4) {
        ihdr->height <<= 8;
        ihdr->height |= this->fileBuffer[reference++];
        index++;
    }


    std::cout << (*ihdr).width << " Width" << std::endl;
    std::cout << (*ihdr).height << " Height" << std::endl;

    // Bit Depth 
    index = 0;
    while(reference < this->fileBuffer.size() && index < 1) 
    {
        ihdr->depth |= this->fileBuffer[reference++];
    }

    std::cout << ihdr->depth << " bit depth" << std::endl;

    // Color Type
    index = 0;
    while(reference < this->fileBuffer.size() && index < 1);
    {
        ihdr->color_type |= this->fileBuffer[reference++];
    }

    std::cout << ihdr->color_type << " color type" << std::endl;

}






// template <typename ChunkType>
// void png_util::Chunk(ChunkType& chunkVar)
// {
//     size_t size = sizeof(ChunkType);    


//     uint8_t csize = 0;  
//     for(int i = 0; i < 4; i++) {
//         csize += fileBuffer[reference++];
//     }
//     std::cout << csize << std::endl;

// };


// template <typename ChunkType>
// Chunk<ChunkType>::Chunk()
// {
//     this->chunk_size = sizeof(T);
// };

