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
                    *dst++ = r1 * wy + r2 * (1 - wy);
                }
            }
        }
    }

    const ScaleMode nearest{L"Ближайший", scaleNearest};
    const ScaleMode bilinear{L"Билинейный", scaleBilinear};
//    const ScaleMode lanczos3;
//    const ScaleMode bcSplines;

    const std::vector<const ScaleMode*> modes = {&nearest, &bilinear};
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
