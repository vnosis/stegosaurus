#include "png_util.h"
#include <iostream>

int main(){
    png_util png;    
    std::string testpng = "static/ldr.png";
    png.loadfile(testpng);
    png.checkfile();
    return 0;
}
