//
// Created by golov on 24.11.2022.
//

#ifndef MEDGRAPHICS_PIXELHSV_H
#define MEDGRAPHICS_PIXELHSV_H


#include "rgba.h"
#include "PixelType.h"
#include "Pixel.h"

class PixelHSV : public Pixel {
public:
    PixelHSV() = default;

    PixelHSV(const PixelHSV& other) = default;

    PixelHSV(double h, double s, double v);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double h{};
    double s{};
    double v{};
};


#endif //MEDGRAPHICS_PIXELHSV_H
