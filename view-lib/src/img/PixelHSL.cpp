//
// Created by golov on 24.11.2022.
//

#include "img/PixelHSL.h"
#include "img/PixelType.h"
#include "img/colorModelConverter.h"

PixelType PixelHSL::getPixelType() const {
    return HSL;
}

rgba PixelHSL::toRGBA() const {
    return toRGBA8(this).toRGBA();
}

PixelHSL::PixelHSL(double h, double s, double l)
        :
        h(h), s(s), l(l) {}
