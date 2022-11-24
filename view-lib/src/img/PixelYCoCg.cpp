//
// Created by golov on 24.11.2022.
//

#include "img/PixelYCoCg.h"
#include "img/colorModelConverter.h"

PixelYCoCg::PixelYCoCg(double y, double co, double cg)
        :
        y{y}, co(co), cg(cg) {}

rgba PixelYCoCg::toRGBA() const {
    return toRGBA8(this).toRGBA();
}

PixelType PixelYCoCg::getPixelType() const {
    return YCoCg;
}

