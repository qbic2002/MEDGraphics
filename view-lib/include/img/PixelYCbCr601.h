//
// Created by golov on 24.11.2022.
//

#ifndef MEDGRAPHICS_PIXELYCBCR601_H
#define MEDGRAPHICS_PIXELYCBCR601_H


#include "img/Pixel.h"

class PixelYCbCr601 : public Pixel {
public:
    PixelYCbCr601() = default;

    PixelYCbCr601(const PixelYCbCr601& other) = default;

    PixelYCbCr601(double y, double cb, double cr);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double y{};
    double cb{};
    double cr{};
};


#endif //MEDGRAPHICS_PIXELYCBCR601_H
