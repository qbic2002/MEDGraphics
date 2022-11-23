//
// Created by danil on 02.11.2022.
//

#ifndef MEDGRAPHICS_PIXELRGB8_H
#define MEDGRAPHICS_PIXELRGB8_H

#include "Pixel.h"

class PixelRGB8 : public Pixel {
public:
    PixelRGB8() = default;

    PixelRGB8(const PixelRGB8& other) = default;

    PixelRGB8(unsigned char r, unsigned char g, unsigned char b)
            :
            r(r), g(g), b(b) {}

    rgba toRGBA() const override {
        return {r, g, b, 255};
    }

    PixelType getPixelType() const override {
        return RGB8;
    }

public:
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
};

#endif //MEDGRAPHICS_PIXELRGB8_H
