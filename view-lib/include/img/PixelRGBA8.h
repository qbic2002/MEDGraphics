//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PIXELRGBA8_H
#define MEDGRAPHICS_PIXELRGBA8_H

#include "Pixel.h"
#include "PixelGray8.h"

class PixelRGBA8 : public Pixel {
public:
    PixelRGBA8() : PixelRGBA8(0, 0, 0, 0) {
    }

    PixelRGBA8(rgba rgba) : rgba(rgba) {
    }

    PixelRGBA8(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {
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


#endif //MEDGRAPHICS_PIXELRGBA8_H
