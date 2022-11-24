//
// Created by golov on 24.11.2022.
//

#include "img/PixelYCbCr709.h"
#include "img/colorModelConverter.h"

PixelYCbCr709::PixelYCbCr709(double y, double cb, double cr)
        :
        y(y), cb(cb), cr(cr) {}

rgba PixelYCbCr709::toRGBA() const {
    return toRGBA8(this).toRGBA();
}

PixelType PixelYCbCr709::getPixelType() const {
    return YCbCr709;
}
