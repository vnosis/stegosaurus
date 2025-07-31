#include "png_util.h"
#include <iostream>
#include <memory>

int main(){
    png_util png;    
    std::string testpng = "static/22.png";
    png.loadfile(testpng);
    png.checkfile();

    std::shared_ptr<pnglib::IHDR> pngIHDR = std::make_shared<pnglib::IHDR>();
    std::shared_ptr<pnglib::PLTE> pngPLTE = std::make_shared<pnglib::PLTE>();
    std::shared_ptr<pnglib::IDAT> pngIDAT = std::make_shared<pnglib::IDAT>();
    
    png.IHDR(pngIHDR);
    png.PLTE(pngPLTE);
    std::cout << png.IDAT(pngIDAT) << std::endl;
    // png.scanline(pngIHDR->color_type, pngIHDR->depth, pngIHDR->width);
    png.Decompress(pngIDAT);
    png.IEND();

    return 0;
}
