#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "core/MyApp.h"

int main([[maybe_unused]] int argc, char** argv) {
    MyApp app;
    app.run(argc, argv, 640, 480, "Hello World", true);
    return 0;
}