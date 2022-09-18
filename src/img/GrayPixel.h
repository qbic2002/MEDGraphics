//
// Created by golov on 17.09.2022.
//

#ifndef MEDGRAPHICS_GRAYPIXEL_H
#define MEDGRAPHICS_GRAYPIXEL_H


#include "Pixel.h"
#include "RGBAPixel.h"

class GrayPixel : public Pixel {
public:
    GrayPixel() = default;

    GrayPixel(int gray) : grayScale(gray) {}

    rgba toRGBA() const override {
        return {grayScale, grayScale, grayScale, 255};
    }

public:
    unsigned char grayScale;
};


#endif //MEDGRAPHICS_GRAYPIXEL_H
