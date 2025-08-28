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
    // png.scanline(pngIHDR->color_type, pngIHDR->depth, pngIHDR->width, pngIHDR->byteperpixel);
    // Doing format first to set ihdr values
    png.SCANLINE_FORMAT(pngIHDR);
    if(png.Decompress(pngIDAT) != 1)
    {
        std::cout << "Decompression Error\n";
        return -1;
    }

    std::cout << pngIDAT->decompressedD.size() << " Decompressed\n";
    if(pngIDAT->decompressedD.size() != png.DcompressSize(pngIDAT, pngIHDR))
    {
        std::cout << "Exiting Program\n";
        return -1;
    }

    int count = 0;
    for(int i = 0; i < pngIDAT->decompressedD.size();i++)
    {
        if(pngIDAT->decompressedD[i] == 255){
            count++;
            std::cout << " 255 " << count << " " << i << std::endl;
        }
    }

    int scanlineSize = pngIHDR->byteperpixel * pngIHDR->width + 1;
    std::cout << "Scanline size " << scanlineSize << std::endl;
    std::vector<std::vector<int>> scanlineRows{};

    int j = 0;
    std::vector<ubyte> scanCol{};
    std::vector<ubyte> filter_Elements{};

    int n_firstElements = pngIDAT->decompressedD.size()/scanlineSize;
     
    std::cout << n_firstElements << " n_firstElements\n" << std::endl;
    int index = 0;
    int i = 1;
    while(i <= 32)
    {
        int scansize = scanlineSize * i;
        std::cout << " NEW SCANLINE \n";
         
        filter_Elements.push_back((int)pngIDAT->decompressedD[index++]);
        for(;index < scansize;)
        {
            std::cout << (int)pngIDAT->decompressedD[index] << " ";
            scanCol.push_back((int)pngIDAT->decompressedD[index++]);
        }
        std::cout << "\n";
        i++;
    }
    for(int i = 0; i < filter_Elements.size(); i++)
    {
        std::cout << (int)filter_Elements[i] << std::endl;
    }
    std::cout << filter_Elements.size() << std::endl;

    // for(int i = 0; i < pngIDAT->decompressedD.size(); i++) 
    // {
    //     // FIX THIS LINE OF CODE 
    //     // Fixed the issue but check to see if there is 
    //     // more to be done 
    //     scanCol.push_back((int)pngIDAT->decompressedD[i]);
    //     if(pngIDAT->decompressedD.size() % scanline != 0)
    //     {
    //         std::cout << std::endl;
    //         std::cout << "NEW SCANLINE\n";
    //         j++;
    //     }
    // }

    // fix this part of the code please 

    // for(int i = 0; i < scanCol.size(); i++)
    // {
    //     scanlineRows.push_back(scanCol);
    // }
    std::cout << "\n";

    std::cout << "Reversed Tesn\n";

    // Fix this reverse segment fault issue
    std::reverse(scanlineRows.begin(), scanlineRows.end());
    std::cout << " Check\n";

    std::cout << scanlineRows.size() << " size of rows" << std::endl;

    for(int i = 0; i < scanlineRows.size(); i++)
    {
        std::cout << scanlineRows[i][0] << std::endl;
        std::cout << "inside loop\n";
    }

    // for(int  i = 0; i < scanlineRows[0].size(); i++) 
    // {
    //     std::cout << scanlineRows[0][i] << std::endl;
    // }

    std::cout << " " << std::endl;

    png.IEND();

    return 0;
}
