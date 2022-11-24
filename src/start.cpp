#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/PixelHSL.h"
#include "img/colorModelConverter.h"
#include "img/PixelHSV.h"

int main([[maybe_unused]] int argc, char** argv) {
//    PixelRGB8 p(255, 173, 255);
//    PixelYCbCr601 p1 = toYCbCr601(&p);
//
//    std::cout << (int)p1.y << " " << (int)p1.cb << " " << (int)p1.cr << "\n";
//
//    rgba rgba = p1.toRGBA();
//
//    std::cout << (int)rgba.r << " " << (int)rgba.g << " " << (int)rgba.b << "\n";
//    return 0;
    MyApp app;
    app.run(argc, argv, 640, 480, "Hello World", true);
    return 0;
}