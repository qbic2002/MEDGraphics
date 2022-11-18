#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "ExpApp.h"

int main(int argc, char** argv) {
    ExpApp app;
    app.run(argc, argv, 640, 480, "Experiment", true);
    return 0;
}