//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBAPIXEL_H
#define MEDGRAPHICS_RGBAPIXEL_H

#include "Pixel.h"

class RGBAPixel : public Pixel {
public:
    RGBAPixel() = default;

    RGBAPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : R(r), G(g), B(b), A(a) {}

    RGBAPixel getGrey() const {
        unsigned char color = (this->R + this->G + this->B) / 3;
        return RGBAPixel(color, color, color, this->A);
    }

    void toGrey() {
        unsigned char color = (this->R + this->G + this->B) / 3;
        this->R = color;
        this->G = color;
        this->B = color;
    }

    RGBAPixel getRGBA() const override {
        return {(*this)};
    }

public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};


#endif //MEDGRAPHICS_RGBAPIXEL_H
