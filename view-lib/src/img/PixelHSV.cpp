//
// Created by golov on 24.11.2022.
//

#include "img/PixelHSV.h"
#include "img/colorModelConverter.h"

PixelHSV::PixelHSV(double h, double s, double v)
        :
        h(h), s(s), v(v) {}

PixelType PixelHSV::getPixelType() const {
    return HSV;
}

rgba PixelHSV::toRGBA() const {
    return toRGBA8(this).toRGBA();
}
