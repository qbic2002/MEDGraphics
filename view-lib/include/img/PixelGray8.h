//
// Created by golov on 17.09.2022.
//

#ifndef MEDGRAPHICS_PIXELGRAY8_H
#define MEDGRAPHICS_PIXELGRAY8_H


#include "Pixel.h"

class PixelGray8 : public Pixel {
public:
    PixelGray8() = default;

    PixelGray8(const PixelGray8& other) = default;

    PixelGray8(int gray)
            :
            grayScale(gray) {}

    rgba toRGBA() const override {
        return {grayScale, grayScale, grayScale, 255};
    }

    PixelType getPixelType() const override {
        return GRAY;
    }

public:
    unsigned char grayScale{};
};


#endif //MEDGRAPHICS_PIXELGRAY8_H
