//
// Created by golov on 24.11.2022.
//

#ifndef MEDGRAPHICS_PIXELYCBCR709_H
#define MEDGRAPHICS_PIXELYCBCR709_H


#include "Pixel.h"

class PixelYCbCr709 : public Pixel {
public:
    PixelYCbCr709() = default;

    PixelYCbCr709(const PixelYCbCr709& other) = default;

    PixelYCbCr709(double y, double cb, double cr);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double y{};
    double cb{};
    double cr{};
};


#endif //MEDGRAPHICS_PIXELYCBCR709_H
