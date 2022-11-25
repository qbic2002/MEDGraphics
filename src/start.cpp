#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/colorModelConverter.h"

int main([[maybe_unused]] int argc, char** argv) {
//    PixelRGB8 p(2, 173, 9);
//    PixelCMY8 p1 = toCMY8(&p);
//
//    std::cout << p1.c << " " << p1.m << " " << p1.y << "\n";
//
//    rgba rgba = p1.toRGBA();
//
//    std::cout << (int)rgba.r << " " << (int)rgba.g << " " << (int)rgba.b << "\n";
//    return 0;
    MyApp app;
    app.run(argc, argv, 1280, 720, "Hello World", true);
    return 0;
}