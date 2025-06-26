#include <gtest/gtest.h>
#include "../src/png_util.h"

TEST(PNGUTILTEST, LoadFileWorks) {
    png_util png;
    std::string testpng = "../static/ldr.png"; 
    EXPECT_TRUE(png.loadfile(testpng));
};

// TEST(PNGUTILTEST, CheckPngFile) {
//     png_util png;
//     EXPECT_TRUE(png.checkfile());
// }

// TEST(PNGUTILTEST, SaveFile) {
//     png_util png;
//     std::string savedfile = "../static/savedfile";
//     EXPECT_TRUE(png.savefile(savedfile));
// };