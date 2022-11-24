//
// Created by golov on 24.11.2022.
//

#ifndef MEDGRAPHICS_PIXELYCOCG_H
#define MEDGRAPHICS_PIXELYCOCG_H


#include "Pixel.h"

class PixelYCoCg : public Pixel {
public:
    PixelYCoCg() = default;

    PixelYCoCg(const PixelYCoCg& other) = default;

    PixelYCoCg(double y, double co, double cg);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    double y{};
    double co{};
    double cg{};
};


#endif //MEDGRAPHICS_PIXELYCOCG_H
