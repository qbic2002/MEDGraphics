//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_PIXELHSL8_H
#define MEDGRAPHICS_PIXELHSL8_H


#include "Pixel.h"
#include <cmath>

class PixelHSL8 : public Pixel {
public:
    PixelHSL8() = default;

    PixelHSL8(const PixelHSL8& other) = default;

    PixelHSL8(double h, double s, double l)
            :
            h(h), s(s), l(l) {}

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double h{};
    double s{};
    double l{};
};


#endif //MEDGRAPHICS_PIXELHSL8_H
