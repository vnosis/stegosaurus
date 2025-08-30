#include "png_util.h"
#include <iostream>
#include <memory>

int main(){
    png_util png;    
    std::string testpng = "static/bas.png";
    png.loadfile(testpng);
    png.checkfile();

    std::shared_ptr<pnglib::IHDR> pngIHDR = std::make_shared<pnglib::IHDR>();
    std::shared_ptr<pnglib::PLTE> pngPLTE = std::make_shared<pnglib::PLTE>();
    std::shared_ptr<pnglib::IDAT> pngIDAT = std::make_shared<pnglib::IDAT>();
    
    png.IHDR(pngIHDR);
    png.PLTE(pngPLTE);
    png.IDAT(pngIDAT);

    // Doing format first to set ihdr values
    png.SCANLINE_FORMAT(pngIHDR);
    if(png.Decompress(pngIDAT) != 1)
    {
        std::cout << "Decompression Error\n";
        return -1;
    }

    std::cout << pngIDAT->decompressedD.size() << " Decompressed Size \n";
    if(pngIDAT->decompressedD.size() != png.DcompressSize(pngIDAT, pngIHDR))
    {
        std::cout << "Exiting Program, Decompressed size and expected size do not match\n";
        return -1;
    }

    png.scanline(pngIHDR, pngIDAT);
    png.InitBPP(pngIHDR);
    png.InitSections(pngIDAT);

    png.ApplyFilters(pngIDAT); 

    std::cout << "\n";

    png.IEND();

    return 0;
}
