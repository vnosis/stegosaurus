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

    // Using index to count 8 bits per index
    if(pnglib::ChunkMap["IHDR"])
    {
        uint8_t index = 0; while(reference < this->fileBuffer.size() && index < 4) { ihdr->width <<= 8; ihdr->width |= this->fileBuffer[reference++]; index++; }
        index = 0;
        while(reference < this->fileBuffer.size() && index < 4) {
            ihdr->height <<= 8;
            ihdr->height |= this->fileBuffer[reference++];
            index++;
        }

        std::cout << (*ihdr).width << " Width" << std::endl;
        std::cout << (*ihdr).height << " Height" << std::endl;

        index = 0;
        ihdr->depth |= this->fileBuffer[reference++];
        std::cout << (int)ihdr->depth << " bit depth" << std::endl;

        ihdr->color_type |= this->fileBuffer[reference++];
        std::cout << (int)ihdr->color_type << " color type" << std::endl;

        ihdr->compression |= this->fileBuffer[reference++];
        std::cout << (int)ihdr->compression << " Compression" << std::endl;

        ihdr->filter |= this->fileBuffer[reference++];
        std::cout << (int)ihdr->filter << " Filter Method" << std::endl;

        ihdr->interlace |= this->fileBuffer[reference++];
        std::cout << (int)ihdr->interlace << " Interlace" << std::endl;
    }
    

}

//PLTE must start after IHDR
// TODO change reference start
int png_util::PLTE() 
{
    this->reference = 0;
    std::queue<ubyte> plte({0x50, 0x4c, 0x54, 0x45});
    std::queue<ubyte> plteQueue{};

    for(int i = 0; i < plte.size(); i++) 
    {
        plteQueue.push(this->fileBuffer[reference++]);
    }

    while(this->reference < this->fileBuffer.size() && plte != plteQueue) {
        plteQueue.pop();
        plteQueue.push(this->fileBuffer[reference++]);
    }

    // Get size of PLTE
    ubyte4 plteSize = 0;
    if(plte == plteQueue)
    {
        ubyte refSize = reference - 8u;
        if(refSize > 0) 
        {
            plteSize = this->fileBuffer[refSize]<<24 | this->fileBuffer[refSize+1]<<16 |
                        this->fileBuffer[refSize+2]<<8 | this->fileBuffer[refSize+3];
        } 
    }
    else
    {
        std::cout << "PLTE is not found\n";
        return ERROR_INVALID_PLTE;
    } 

    std::cout << plteSize << " PLTE SIZE\n";
    if(plteSize % 3 != 0)
    {
        return ERROR_INVALID_PLTE;
    }

    std::vector<ubyte> dataBuffer(plteSize, 0);
    while(plteSize > 0 && reference < this->fileBuffer.size()) {
        dataBuffer.push_back(this->fileBuffer[reference++]);
        plteSize--;
    }


    return SUCCESS;
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

