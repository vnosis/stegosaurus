#include "png_util.h"
#include <fstream> 
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <queue>
#include <zlib.h>

size_t png_util::getIndex(int x, int y) const
{
    return size_t();
}

void png_util::InitBPP(std::shared_ptr<pnglib::IHDR> ihdr)
{
    this->bytesPerPixel = ihdr->byteperpixel;
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
    std::cout << reference << " END\n";

    return SUCCESS;
};

int png_util::DcompressSize(std::shared_ptr<pnglib::IDAT> idat, std::shared_ptr<pnglib::IHDR> ihdr) 
{
    ubyte2 ddata = ihdr->height * (1 + ihdr->width * ihdr->byteperpixel);
    std::cout << (int)ddata << " Expected Size\n";
    return (int)ddata;
}

int png_util::scanline(std::shared_ptr<pnglib::IHDR> ihdr, std::shared_ptr<pnglib::IDAT> idat)
{
    std::cout << "<--- Creating Scanline --->\n";
    int scanline_Size = ihdr->byteperpixel * ihdr->width + 1;
    int rows = idat->decompressedD.size()/scanline_Size;
    std::cout << rows << " Number of rows\n";

    std::vector<std::vector<ubyte>> filtered_rows(rows);
    std::vector<ubyte> filter_functions{};

    int index = 0;
    int i = 1;

    while(i <= rows)
    {
        int scansize = scanline_Size * i;

        filter_functions.push_back((int)idat->decompressedD[index++]);
        for(;index < scansize;)
        {
            filtered_rows[i-1].push_back(idat->decompressedD[index++]);
        }
        i++;
    }

    for(int i = 0; i < filter_functions.size(); i++)
    {
        std::cout << (int)filter_functions[i] << " " << std::endl;
    }

    for(int i = 0; i < filtered_rows.size(); i++)
    {
        std::cout << i << " NEW ROW \n";
        for(int j = 0; j < filtered_rows[i].size(); j++)
        {

            std::cout << (int)filtered_rows[i][j] << " ";
        }
    }

    idat->filtered_rows = filtered_rows;
    idat->filterFunctions = filter_functions;

    return SUCCESS;
}

int png_util::ApplyFilters(std::shared_ptr<pnglib::IDAT> idat)
{
    std::cout << "<--- Apply Filter --->\n";
    int rows = idat->filterFunctions.size();
    for(int row = 0; row < rows; row++)
    {

        switch(idat->filterFunctions[row])
        {
            case 0:
                break;
            case 1:
                Filter_One(idat, row);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                throw std::invalid_argument("Invalid Function Element");
        }
    }
    return SUCCESS; 
}

int png_util::Filter_One(std::shared_ptr<pnglib::IDAT> idat, int row)
{
    std::cout << "<--- FILTER ONE ---\n";
    int sections = idat->filtered_rows[0].size()/this->bytesPerPixel;

    this->rawData = std::vector<std::vector<ubyte>>(idat->filtered_rows.size());
    std::cout << " raw Data " << this->rawData[row].size() << std::endl;
    for(int i = 0; i < this->bytesPerPixel; i++)
    {
        this->rawData[row].push_back(idat->filtered_rows[row][i]);
    }

    for(int section = 1; section < sections; section+=this->bytesPerPixel)
    {
        int start = section * this->bytesPerPixel;
        int end = start + this->bytesPerPixel;

        this->rawData[row].insert(this->rawData[row].begin(), 
                                    idat->filtered_rows[row].begin() + start,
                                    idat->filtered_rows[row].begin() + end);
    }

    std::cout << this->rawData[row].size() << std::endl;

        std::cout << "test" << std::endl;
    for(int i = 0; i < this->rawData[row].size(); i++)
    {
        std::cout << this->rawData[row][i] << std::endl;
    }

    return 0;
}

int png_util::IDAT(std::shared_ptr<pnglib::IDAT> idat)
{
    
    std::cout << "<---IDAT--->\n";
    this->reference = 0;

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
        std::cout << "IDAT not found\n";
        std::cout << "REFERENCE " << reference << std::endl;
        return ERROR_INVALID_IDAT;
    }

    int refStart = reference - 8;
    if(refStart > 0) 
    {
        idat->size = this->fileBuffer[refStart]<<24 |
                     this->fileBuffer[refStart + 1]<<16 |
                     this->fileBuffer[refStart + 2]<<8 |
                     this->fileBuffer[refStart + 3];
    }
    else    
    {
        std::cout << "IDAT not found\n";
        std::cout << "REFERENCE " << reference << std::endl;
        return ERROR_INVALID_IDAT;
    }

    for(int i = 0; i < idat->size; i++)
    {
        idat->compressedD.push_back(this->fileBuffer[reference++]);
        //std::cout << (int)this->fileBuffer[reference] << std::endl;
    }
    std::cout << this->fileBuffer[reference] << std::endl;

    std::cout << "IDAT found" << std::endl;

    std::cout << idat->size << " IDAT size" << std::endl;

    return SUCCESS;
}

//FIX ALL DECOMPRESSION ISSUES

int png_util::Decompress(std::shared_ptr<pnglib::IDAT> idat) 
{
    constexpr size_t CHUNK_SIZE = 4128;
    //Todo If chunk map has positive idat value then decompress
    z_stream strm;
    strm.next_in = const_cast<Bytef*>(idat->compressedD.data());
    strm.avail_in = idat->compressedD.size();
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;

    if(inflateInit(&strm) != Z_OK)
    {
        throw std::runtime_error("InflateInit Failed!");
    }

    std::vector<ubyte> out_buffer(CHUNK_SIZE);

    int ret;
    do
    {
        strm.next_out = out_buffer.data();
        strm.avail_out = out_buffer.size();

        ret = inflate(&strm, Z_NO_FLUSH);

        if(ret != Z_OK && ret != Z_STREAM_END) 
        {
            inflateEnd(&strm);
            throw std::runtime_error("inflate failed with error: " + std::to_string(ret));
        }

        size_t bytes_decompressed = out_buffer.size() - strm.avail_out;
        idat->decompressedD.insert(idat->decompressedD.end(), out_buffer.begin(), out_buffer.begin() + bytes_decompressed);
    } while(ret != Z_STREAM_END);

    inflateEnd(&strm);

    //Expected idat decompressed data

    return SUCCESS;
}

int png_util::SCANLINE_FORMAT(std::shared_ptr<pnglib::IHDR> ihdr)
{
    std::cout << "<---SCANLINE FORMAT--->\n";
    this->scanlineLength = ihdr->width * pnglib::ScanSample[ihdr->color_type] * ihdr->depth / 8;
    
    int colorType = (int)ihdr->color_type;

    std::cout << colorType << std::endl;
    switch((int)colorType) 
    {
        case 0: ihdr->byteperpixel = 1 * ihdr->depth; break;
        case 2: ihdr->byteperpixel = 3 * ihdr->depth; break;
        case 3: ihdr->byteperpixel = 1 * ihdr->depth; break;
        case 4: ihdr->byteperpixel = 2 * ihdr->depth; break;
        case 6: ihdr->byteperpixel = 4 * ihdr->depth; break;
        default: throw std::invalid_argument("No colortype");
    }

    ihdr->byteperpixel /= 8;
    
    std::cout << ihdr->byteperpixel << "byteperpixel\n";

    this->bytesPerRow = ihdr->width * ihdr->byteperpixel;

    std::cout << this->bytesPerRow << " Bytesperow\n";
    return SUCCESS;
}