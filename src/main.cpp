#include "png_util.h"
#include <iostream>
#include <memory>

int main(){
    png_util png;    
    std::string testpng = "static/ldr.png";
    png.loadfile(testpng);
    png.checkfile();

    std::shared_ptr<pnglib::Chunk_IHDR> pngihdr = std::make_shared<pnglib::Chunk_IHDR>();

    png.IHDR(pngihdr);


    return 0;
}
