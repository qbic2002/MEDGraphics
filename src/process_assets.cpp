//
// Created by danil on 19.09.2022.
//

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <string>
#include <iostream>
#include <filesystem>
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

string root = R"(C:\Workspace\University\Semester-5\Graphics\cpp-experiments\assets\icons\)";

typedef unsigned char uchar;

struct rgb {
    uchar r;
    uchar g;
    uchar b;
};

struct rgba {
    rgb rgb;
    uchar a;

    bool operator==(const rgba& other) const {
        return rgb.r == other.rgb.r && rgb.g == other.rgb.g && rgb.b == other.rgb.b && a == other.a;
    }
};

rgba* ensureRgba(uchar* data, int width, int height, int channels) {
    switch (channels) {
        case 3: {
            rgb* rgbArray = (rgb*) data;
            rgba* result = new rgba[width * height];
            for (int i = 0; i < width * height; i++) {
                result[i] = {rgbArray[i], 255};
            }
            free(data);
            return result;
        }
        case 4: {
            rgba* rgbaArray = (rgba*) data;
            rgba* result = new rgba[width * height];
            for (int i = 0; i < width * height; i++) {
                result[i] = rgbaArray[i];
            }
            free(data);
            return result;
        }
        default:
            cerr << "Unexpected channels count: " << channels << endl;
            throw exception();
    }
}

rgba* ensureMask(rgba* data, int width, int height) {
    bool containsBlackPixel = false;
    rgba black = {.rgb = {0, 0, 0}, .a = 255};
    for (int i = 0; i < width * height; i++) {
        if (data[i] == black) {
            containsBlackPixel = true;
            break;
        }
    }
    if (!containsBlackPixel)
        return data;
    rgba white = {.rgb = {255, 255, 255}, .a = 255};
    for (int i = 0; i < width * height; i++) {
        if (data[i] == black) {
            data[i] = white;
        } else if (data[i] == white) {
            data[i] = {0, 0, 0, 0};
        }
    }
    return data;
}

void process(const string& name) {
    int width, height, channels;
    uchar* data = stbi_load(name.c_str(), &width, &height, &channels, 0);

    rgba* result = ensureRgba(data, width, height, channels);
    result = ensureMask(result, width, height);

    stbi_write_png(name.c_str(), width, height, 4, result, width * 4);

    delete[] result;
}

int main() {
    for (const auto& file: filesystem::directory_iterator(root))
        process(file.path().string());
}