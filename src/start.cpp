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
#include "utils/random.h"
#include <zlib.h>

int main([[maybe_unused]] int argc, char** argv) {
//
//    std::vector<unsigned char> v;
//    for (int i = 0; i < 10000; ++i){
//        v.push_back(i);
//    }
//    std::vector<unsigned char> res = zlib::deflate(v);
//    std::vector<unsigned char> r = zlib::inflate(res);
//
//
//    return 0;
    MyApp app;
    app.run(argc, argv, 1280, 720, "Hello World", true);
    return 0;
}