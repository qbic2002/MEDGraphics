//
// Created by golov on 01.12.2022.
//

#include <stdexcept>
#include <cmath>
#include "../include/img/DitheringMethods.h"
#include <utils/random.h>

OrderedDithering orderedDithering;
RandomDithering randomDithering;
FloydSteinbergDithering floydSteinbergDithering;
AtkinsonDithering atkinsonDithering;

const DitheringMethod* ditheringMethods[] = {&orderedDithering, &randomDithering, &floydSteinbergDithering,
                                             &atkinsonDithering};

const DitheringMethod* findDitheringMethod(DitheringMethodEnum ditheringMethod) {
    return ditheringMethods[ditheringMethod];
}

constexpr float* getMatrix(int n) {
    if (n < 2)
        throw std::runtime_error("wrong param n");
    if (n == 2)
        return new float[4]{0, 2, 3, 1};

    auto* matrix = new float[n * n];

    float* prevMatrix = getMatrix(n / 2);

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            matrix[j * n + i] = prevMatrix[(n / 2) * (j / 2) + (i / 2)]
                                + prevMatrix[(n / 2) * (j % 2) + (i % 2)] * 4;
        }
    }

    delete[] prevMatrix;

    return matrix;
}

inline float nearestColorF(float value, int totalColors) {
    if (value >= 1)
        return 1;
    if (value < 0)
        return 0;

    return std::floor(value * totalColors) / (totalColors - 1);
}

void OrderedDithering::dither(int bits, const float* src, float* dst, int stride, int width, int height) const {
    if (bits > 8 || bits < 1) {
        throw std::runtime_error("wrong number of bits in dithering");
    }

    int colors = pow(2, bits);
    float* m = getMatrix(8);
    for (int i = 0; i < 64; i++)
        m[i] = m[i] / 64 - 0.5;

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            *dst = nearestColorF(*src + 1.0f / bits * m[8 * (h % 8) + (w % 8)], colors);
            src += stride;
            dst += stride;
        }
    }

    delete[] m;
}

void RandomDithering::dither(int bits, const float* src, float* dst, int stride, int width, int height) const {
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            float threshold = utils::randomFloat();
            *dst = *src < threshold ? 0 : 1;
            dst += stride;
            src += stride;
        }
    }
}

void FloydSteinbergDithering::dither(int bits, const float* src, float* dst, int stride, int width, int height) const {
    int colors = pow(2, bits);

    auto* tmpData = new float[width * height];
    for (int i = 0; i < width * height; ++i) {
        tmpData[i] = src[i * stride];
    }

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            float value = nearestColorF(tmpData[h * width + w], colors);
            float error = (tmpData[h * width + w] - value) / 16;

            if (w != width - 1) {
                tmpData[h * width + w + 1] += 7 * error;
            }
            if ((w != 0) && (h != height - 1)) {
                tmpData[(h + 1) * width + w - 1] += 3 * error;
            }
            if (h != height - 1) {
                tmpData[(h + 1) * width + w] += 5 * error;
            }
            if ((w != width - 1) && (h != height - 1)) {
                tmpData[(h + 1) * width + w] += error;
            }

            *dst = value;
            dst += stride;
        }
    }

    delete[] tmpData;
}

void AtkinsonDithering::dither(int bits, const float* src, float* dst, int stride, int width, int height) const {
    int colors = pow(2, bits);

    auto* tmpData = new float[width * height];
    for (int i = 0; i < width * height; ++i) {
        tmpData[i] = src[i * stride];
    }

    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            float value = nearestColorF(tmpData[h * width + w], colors);
            float error = (tmpData[h * width + w] - value) / 8;

            if (w != width - 1) {
                tmpData[h * width + w + 1] += error;
            }
            if ((w != 0) && (h != height - 1)) {
                tmpData[(h + 1) * width + w - 1] += error;
            }
            if (h != height - 1) {
                tmpData[(h + 1) * width + w] += error;
            }
            if ((w != width - 1) && (h != height - 1)) {
                tmpData[(h + 1) * width + w] += error;
            }
            if (h < height - 2) {
                tmpData[(h + 2) * width + w] += error;
            }
            if (w < width - 2) {
                tmpData[h * width + w + 2] += error;
            }

            *dst = value;
            dst += stride;
        }
    }

    delete[] tmpData;
}
