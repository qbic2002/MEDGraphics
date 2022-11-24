#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/colorModelConverter.h"

int main([[maybe_unused]] int argc, char** argv) {
//    PixelRGB8 p(255, 173, 9);
//    PixelYCbCr709 p1 = toYCbCr709(&p);
//
//    std::cout << p1.y << " " << p1.cb << " " << p1.cr << "\n";
//
//    rgba rgba = p1.toRGBA();
//
//    std::cout << (int)rgba.r << " " << (int)rgba.g << " " << (int)rgba.b << "\n";
//    return 0;
    MyApp app;
    app.run(argc, argv, 640, 480, "Hello World", true);
    return 0;
}