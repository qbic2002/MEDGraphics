//
// Created by golov on 01.12.2022.
//

#include <stdexcept>
#include <cmath>
#include <iostream>
#include <random>

#include "../include/img/DitheringMethods.h"

OrderedDithering orderedDithering;
RandomDithering randomDithering;
FloydSteinbergDithering floydSteinbergDithering;

const DitheringMethod* ditheringMethods[] = {&orderedDithering, &randomDithering, &floydSteinbergDithering};

const DitheringMethod* findDitheringMethod(DitheringMethods ditheringMethod) {
    return ditheringMethods[ditheringMethod];
}

constexpr int* getMatrix(int n) {
    if (n < 2)
        throw std::runtime_error("wrong param n");
    if (n == 2)
        return new int[4]{0, 2, 3, 1};

    int* matrix = new int[n * n];

    int* prevMatrix = getMatrix(n / 2);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[j * n + i] = prevMatrix[(n / 2) * (j % 2) + (i % 2)] * 4 +
                                prevMatrix[(n / 2) * (int) floor(j / 2.0) + (int) floor(i / 2.0)];

        }
    }

    delete[] prevMatrix;

    return matrix;
}

inline unsigned char nearestColor(int color, int totalColors) {
    if (color > 255)
        return 255;
    if (color < 0)
        return 0;


    float step = 255.0 / (totalColors - 1);
    int temp = roundf(color / step);
    int result = (temp * step);

    if (result > 255)
        result = 255;
    if (result < 0)
        result = 0;
    return (unsigned char) result;
}

void OrderedDithering::dither(int bits, const rgba* data, rgba* dest, int height, int width) const {
    if (bits > 8 || bits < 1) {
        throw std::runtime_error("wrong number of bits in dithering");
    }

    int colors = pow(2, bits);
    int* m = getMatrix(8);

    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            unsigned char r = nearestColor(
                    data[(h * width + w)].r + 255.0 / bits * ((m[8 * (h % 8) + (w % 8)] / 64.0) - 0.5),
                    colors);
            unsigned char g = nearestColor(
                    data[(h * width + w)].g + 255.0 / bits * ((m[8 * (h % 8) + (w % 8)] / 64.0) - 0.5),
                    colors);
            unsigned char b = nearestColor(
                    data[(h * width + w)].b + 255.0 / bits * ((m[8 * (h % 8) + (w % 8)] / 64.0) - 0.5),
                    colors);

            dest[(h * width + w)].r = r;
            dest[(h * width + w)].g = g;
            dest[(h * width + w)].b = b;
            dest[(h * width + w)].a = data[(h * width + w)].a;
        }

    }

    delete[] m;
}

void RandomDithering::dither(int bits, const rgba* data, rgba* dest, int height, int width) const {
    srand((unsigned) time(nullptr));
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {


            unsigned char treshold = rand() % 255;
//            std::cout << (int)treshold << "\n";
            unsigned char r = data[(h * width + w)].r < treshold ? 0 : 255;
            unsigned char g = (data[(h * width + w)].g < treshold) ? 0 : 255;
            unsigned char b = (data[(h * width + w)].b < treshold) ? 0 : 255;

            dest[(h * width + w)].r = r;
            dest[(h * width + w)].g = g;
            dest[(h * width + w)].b = b;
            dest[(h * width + w)].a = data[(h * width + w)].a;
        }

    }
}

void FloydSteinbergDithering::dither(int bits, const rgba* data, rgba* dest, int height, int width) const {
    int colors = pow(2, bits);

    typedef struct {
        int r, g, b, a;
    } rgbaInt;

    auto* newData = new rgbaInt[width * height];

    for (int i = 0; i < height * width; ++i) {
        newData[i].r = data[i].r;
        newData[i].g = data[i].g;
        newData[i].b = data[i].b;
    }

    for (int h = height - 1; h >= 0; --h) {
        for (int w = 0; w < width; ++w) {

            unsigned char r = nearestColor(newData[(h * width + w)].r, colors);
            unsigned char g = nearestColor(newData[(h * width + w)].g, colors);
            unsigned char b = nearestColor(newData[(h * width + w)].b, colors);

            if (w != width - 1) {
                newData[(h * width + w + 1)].r += roundf((newData[h * width + w].r - r) * 7.0 / 16.0);
                newData[(h * width + w + 1)].g += roundf((newData[h * width + w].g - g) * 7.0 / 16.0);
                newData[(h * width + w + 1)].b += roundf((newData[h * width + w].b - b) * 7.0 / 16.0);
            }
            if ((w != 0) && (h != 0)) {
                newData[((h - 1) * width + w - 1)].r += roundf((newData[h * width + w].r - r) * 3.0 / 16.0);
                newData[((h - 1) * width + w - 1)].g += roundf((newData[h * width + w].g - g) * 3.0 / 16.0);
                newData[((h - 1) * width + w - 1)].b += roundf((newData[h * width + w].b - b) * 3.0 / 16.0);
            }
            if ((h != 0)) {
                newData[((h - 1) * width + w)].r += roundf((newData[h * width + w].r - r) * 5.0 / 16.0);
                newData[((h - 1) * width + w)].g += roundf((newData[h * width + w].g - g) * 5.0 / 16.0);
                newData[((h - 1) * width + w)].b += roundf((newData[h * width + w].b - b) * 5.0 / 16.0);
            }

            if ((w != width - 1) && (h != 0)) {
                newData[((h - 1) * width + w + 1)].r += roundf((newData[h * width + w].r - r) * 1.0 / 16.0);
                newData[((h - 1) * width + w + 1)].g += roundf((newData[h * width + w].g - g) * 1.0 / 16.0);
                newData[((h - 1) * width + w + 1)].b += roundf((newData[h * width + w].b - b) * 1.0 / 16.0);
            }

            dest[(h * width + w)].r = r;
            dest[(h * width + w)].g = g;
            dest[(h * width + w)].b = b;
            dest[(h * width + w)].a = data[(h * width + w)].a;
        }
    }

    delete[] newData;
}
