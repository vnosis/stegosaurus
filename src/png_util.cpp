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

int png_util::IHDR(std::shared_ptr<pnglib::IHDR> ihdr) 
{
    std::cout << "<---IHDR--->\n";
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

    if(fileQ != ihdrQ)
    {
        return ERROR_INVALID_IHDR;
    }

    // Find ihdrdata size
    int ihdrSizeOffsetRef = reference - 8; 
    ubyte4 ihdrDataSize{};

    if(ihdrSizeOffsetRef > 0) 
    {
        ihdrDataSize = this->fileBuffer[ihdrSizeOffsetRef] <<24 | 
                         this->fileBuffer[ihdrSizeOffsetRef + 1] <<16 |  
                         this->fileBuffer[ihdrSizeOffsetRef + 2] <<8 | 
                         this->fileBuffer[ihdrSizeOffsetRef + 3];
    }

    ihdr->ihdrSize = ihdrDataSize;

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
    
    return SUCCESS;

}

//PLTE must start after IHDR
// TODO change reference start
int png_util::PLTE(std::shared_ptr<pnglib::PLTE> pngPLTE) 
{
    std::cout << "<----PLTE--->\n";
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

    std::cout << plteSize/3 << " PLTE SIZE\n";
    if(plteSize % 3 != 0)
    {
        return ERROR_INVALID_PLTE;
    }

    uint8_t plteSeries = plteSize / 3;
    uint8_t index = 0;
    std::vector<ubyte> rgb{};
    while(index < plteSeries && reference < this->fileBuffer.size()) 
    {
        for(int i = 0; i < 3; i++) 
        {
            rgb.push_back(this->fileBuffer[reference++]);
        }
        pngPLTE->rgbData.push_back(rgb);
        index++;
    }

    return SUCCESS;
}

int png_util::IEND() 
{
    std::cout << "<---IEND--->\n";

    std::queue<ubyte> iendQ{};
    for(int i = 0;i < 4; i++) 
    {
        iendQ.push(this->fileBuffer[reference++]);
    }

    while(reference < this->fileBuffer.size() && iendQ != pnglib::IENDQ) 
    {
        iendQ.pop();
        iendQ.push(this->fileBuffer[reference++]);
    }

    if(reference > this->fileBuffer.size())
    {
        return ERROR_INVALID_IEND;
    }

    return SUCCESS;
};

int png_util::IDAT(std::shared_ptr<pnglib::IDAT> idat)
{
    
    std::cout << "<---IDAT--->\n";

    std::queue<ubyte> idatQ{};
    for(int i = 0; i < 4; i++) 
    {
        idatQ.push(this->fileBuffer[reference++]);
    }

    while(reference < this->fileBuffer.size() && idatQ != pnglib::IDATQ)
    {
        idatQ.pop();
        idatQ.push(this->fileBuffer[reference++]);
    }
    if(idatQ != pnglib::IDATQ)
    {
        return ERROR_INVALID_IDAT;
    }

    ubyte refStart = reference - 8u;
    std::cout << reference <<  " ref" << std::endl;
    if(refStart > 0) 
    {
        std::cout << this->fileBuffer[refStart] << std::endl;
        idat->size = this->fileBuffer[refStart]<<24 |
                     this->fileBuffer[refStart + 1]<<16 |
                     this->fileBuffer[refStart + 2]<<8 |
                     this->fileBuffer[refStart + 3];
    }
    else    
        return ERROR_INVALID_IDAT;

    std::cout << "IDAT found" << std::endl;

    std::cout << idat->size << " IDAT size" << std::endl;

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

