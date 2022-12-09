#define STB_IMAGE_IMPLEMENTATION
#define ZLIB_WINAPI

#include <stb_image.h>
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/colorModelConverter.h"
#include "img/PNGImage.h"
#include "utils/file.h"
#include "img/pngUtils.h"
#include "utils/zlib_utils.h"
#include <zlib.h>

int main([[maybe_unused]] int argc, char** argv) {


//    std::vector<char> res = zlib::inflate("123456", 6);
//    for (const auto& item: res){
//        std::cout << (int)item;
//    }
//    std::cout << "\n";
//    auto v = utils::readAllBytes(R"(D:\CPP\cg22-project-MEDGraphics\assets\test.png)");
//    PNGImage p = png::readPNGImageFromMemory(v.data(), v.size());
//
//    return 0;
    MyApp app;
    app.run(argc, argv, 1280, 720, "Hello World", true);
    return 0;
}