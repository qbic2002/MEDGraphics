//
// Created by danil on 15.12.2022.
//

#include <cmath>
#include <iostream>
#include "img/ImageProcessing.h"

namespace img::scale {
    const float* getPixel(const float* src, int x, int y, int width, int height, int stride) {
        if (x < 0)
            x = 0;
        if (width <= x)
            x = width - 1;
        if (y < 0)
            y = 0;
        if (height <= y)
            y = height - 1;
//        if ((x < 0) || (width <= x) || (y < 0) || (height <= y)) {
//            static float empty[4] = {0, 0, 0, 0};
//            return empty;
//        }
        return src + ((y * width) + x) * stride;
    }

    void scaleNearest(const float* src, int srcWidth, int srcHeight,
                      float* dst, int dstWidth, int dstHeight,
                      int stride, float centerShiftX, float centerShiftY) {
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                float mappedX = (x + 0.5f) * srcWidth / dstWidth - centerShiftX - 0.5;
                float mappedY = (y + 0.5f) * srcHeight / dstHeight - centerShiftY - 0.5;
                int srcX = std::round(mappedX);
                int srcY = std::round(mappedY);
                const float* value = getPixel(src, srcX, srcY, srcWidth, srcHeight, stride);
                for (int i = 0; i < stride; i++) {
                    *dst++ = value[i];
                }
            }
        }
    }

    void scaleBilinear(const float* src, int srcWidth, int srcHeight,
                       float* dst, int dstWidth, int dstHeight,
                       int stride, float centerShiftX, float centerShiftY) {
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                float mappedX = (x + 0.5f) * srcWidth / dstWidth - centerShiftX - 0.5;
                float mappedY = (y + 0.5f) * srcHeight / dstHeight - centerShiftY - 0.5;

                int x1 = std::floor(mappedX);
                int y1 = std::floor(mappedY);
                int x2 = x1 + 1;
                int y2 = y1 + 1;
                /*
                 * 1 - 2
                 * |   |
                 * 3 - 4
                 */
                float wx = x2 - mappedX;
                float wy = y2 - mappedY;

                const float* value1 = getPixel(src, x1, y1, srcWidth, srcHeight, stride);
                const float* value2 = getPixel(src, x2, y1, srcWidth, srcHeight, stride);
                const float* value3 = getPixel(src, x1, y2, srcWidth, srcHeight, stride);
                const float* value4 = getPixel(src, x2, y2, srcWidth, srcHeight, stride);
                for (int i = 0; i < stride; i++) {
                    float r1 = value1[i] * wx + value2[i] * (1 - wx);
                    float r2 = value3[i] * wx + value4[i] * (1 - wx);
                    *dst++ = std::clamp(r1 * wy + r2 * (1 - wy), 0.0f, 1.0f);
                }
            }
        }
    }

    float calcLanczos(float x, float a) {
        if (x == 0)
            return 1;
        if (x < -a || a <= x)
            return 0;
        float piXTimes = M_PI * x;
        return a * std::sin(piXTimes) * std::sin(piXTimes / a) / (piXTimes * piXTimes);
    }

    void scaleLanczos3(const float* src, int srcWidth, int srcHeight,
                       float* dst, int dstWidth, int dstHeight,
                       int stride, float centerShiftX, float centerShiftY) {
        const float a = 3;
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                float mappedX = (x + 0.5f) * srcWidth / dstWidth - centerShiftX - 0.5;
                float mappedY = (y + 0.5f) * srcHeight / dstHeight - centerShiftY - 0.5;

                int intLeft = std::floor(mappedX - a);
                int intRight = std::ceil(mappedX + a);
                int intTop = std::floor(mappedY - a);
                int intBottom = std::ceil(mappedY + a);

                float xWeights[intRight - intLeft + 1];
                for (int i = intLeft; i <= intRight; i++) {
                    xWeights[i - intLeft] = calcLanczos(mappedX - i, a);
                }
                float yWeights[intBottom - intTop + 1];
                for (int i = intTop; i <= intBottom; i++) {
                    yWeights[i - intTop] = calcLanczos(mappedY - i, a);
                }

                float weight = 0;
                float acc[stride];
                for (int i = 0; i < stride; i++) {
                    acc[i] = 0;
                }
                for (int y = intTop; y <= intBottom; y++) {
                    for (int x = intLeft; x <= intRight; x++) {
                        float curW = xWeights[x - intLeft] * yWeights[y - intTop];
                        const float* pixel = getPixel(src, x, y, srcWidth, srcHeight, stride);
                        for (int i = 0; i < stride; i++) {
                            acc[i] += pixel[i] * curW;
                        }
                        weight += curW;
                    }
                }

                for (int i = 0; i < stride; i++) {
                    *dst++ = std::clamp(acc[i] / weight, 0.0f, 1.0f);
                }
            }
        }
    }

    const ScaleMode nearest{L"Ближайший", scaleNearest};
    const ScaleMode bilinear{L"Билинейный", scaleBilinear};
    const ScaleMode lanczos3{L"Фильтр Ланцоша", scaleLanczos3};
//    const ScaleMode bcSplines;

    const std::vector<const ScaleMode*> modes = {&nearest, &bilinear, &lanczos3};
}

void img::histogram(const float* src, int stride, int length, int* dst, int dstLength, int& topValuesCount) {
    for (int i = 0; i < dstLength; i++) {
        dst[i] = 0;
    }
    for (int i = 0; i < length; i++) {
        int index = (int) *src * dstLength;
        if (index == dstLength)
            index = dstLength - 1;
        dst[i] += index;
        src += stride;
    }
    topValuesCount = 0;
    for (int i = 0; i < dstLength; i++) {
        if (dst[i] > topValuesCount)
            topValuesCount = dst[i];
    }
}
