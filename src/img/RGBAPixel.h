//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBAPIXEL_H
#define MEDGRAPHICS_RGBAPIXEL_H

#include "Pixel.h"

class RGBAPixel : public Pixel {
public:
    RGBAPixel() : RGBAPixel(0, 0, 0, 0) {
    }

    RGBAPixel(rgba rgba) : rgba(rgba) {
    }

    RGBAPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {
    }

    RGBAPixel toGray() const {
        unsigned char color = (r + g + b) / 3;
        return {color, color, color, this->a};
    }

    rgba toRGBA() const override {
        return rgba;
    }

public:
    union {
        rgba rgba;
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
    };
};


#endif //MEDGRAPHICS_RGBAPIXEL_H
