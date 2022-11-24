//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_PIXELHSL_H
#define MEDGRAPHICS_PIXELHSL_H


#include "Pixel.h"

class PixelHSL : public Pixel {
public:
    PixelHSL() = default;

    PixelHSL(const PixelHSL& other) = default;

    PixelHSL(double h, double s, double l);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double h{};
    double s{};
    double l{};
};


#endif //MEDGRAPHICS_PIXELHSL_H
