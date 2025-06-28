#include <gtest/gtest.h>
#include "../src/png_util.h"

png_util png;

TEST(PNGUTILTEST, LoadFileWorks) {
    std::string testpng = "static/ldr.png"; 
    EXPECT_TRUE(png.loadfile(testpng));
};

TEST(PNGUTILTEST, SaveFile) {
    std::string savedfile = "static/savedfile.png";
    EXPECT_TRUE(png.savefile(savedfile));
};

TEST(PNGUTILTEST, CheckPngFile) {
    EXPECT_TRUE(png.checkfile());
};

