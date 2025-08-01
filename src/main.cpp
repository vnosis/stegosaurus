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
    std::cout << png.IDAT(pngIDAT) << std::endl;
    png.scanline(pngIHDR->color_type, pngIHDR->depth, pngIHDR->width, pngIHDR->byteperpixel);
    png.Decompress(pngIDAT);


    std::cout << pngIDAT->decompressedD.size() << " Decompressed\n";


    int count = 0;
    for(int i = 0; i < pngIDAT->decompressedD.size();i++)
    {
        if(pngIDAT->decompressedD[i] == 255){
            count++;
            std::cout << " 255 " << count << " " << i << std::endl;

        }
    }

    //Exptected decompressed data
    std::cout << pngIHDR->width << " " << pngIHDR->height << " " << pngIHDR->byteperpixel << std::endl;
    ubyte2 ddata = pngIHDR->height * (1 + pngIHDR->width * pngIHDR->byteperpixel);
    std::cout << (int)ddata <<  " ddata\n";
    int scanline = pngIHDR->byteperpixel * pngIHDR->width + 1;
    std::cout << "Scanline size " << scanline << std::endl;
    std::vector<std::vector<ubyte>> scanlineRows{};

    int j = 0;
    for(int i = 0; i < (int)ddata; i++) 
    {
        scanlineRows[j].push_back((int)pngIDAT->decompressedD[i]);
        std::cout << (int)pngIDAT->decompressedD[i] << " ";
        if(i % scanline == 0)
        {
            std::cout << std::endl;
            std::cout << "NEW SCANLINE\n";
            j++;
        }
    }


    std::cout << "Reversed Tesn\n";

    std::reverse(scanlineRows.begin(), scanlineRows.end());
    for(int  i = 0; i < scanlineRows[0].size(); i++) 
    {
        std::cout << scanlineRows[0][i] << std::endl;
    }

    std::cout << " " << std::endl;

    png.IEND();

    return 0;
}
