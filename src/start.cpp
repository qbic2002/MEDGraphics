#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "core/MyApp.h"
#include "img/PixelRGB8.h"
#include "img/colorModelConverter.h"

int main([[maybe_unused]] int argc, char** argv) {
//    OrderedDithering d;
//    d.dither(8, nullptr, 1, 1);
//
//    return 0;
    MyApp app;
    app.run(argc, argv, 1280, 720, "Hello World", true);
    return 0;
}