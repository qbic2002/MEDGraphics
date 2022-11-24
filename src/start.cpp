#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/PixelHSL.h"
#include "img/colorModelConverter.h"

int main([[maybe_unused]] int argc, char** argv) {
//    PixelRGB8 p(179, 99, 17);
//    PixelHSL p1 = toHSL8(&p);
//
//    std::cout << p1.h << " " << p1.s << " " << p1.l << "\n";
//
//    rgba rgba = p1.toRGBA();
//
//    std::cout << (int)rgba.r << " " << (int)rgba.g << " " << (int)rgba.b << "\n";
//    return 0;
    MyApp app;
    app.run(argc, argv, 640, 480, "Hello World", true);
    return 0;
}