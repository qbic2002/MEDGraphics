//
// Created by golov on 24.11.2022.
//

#include "img/PixelYCbCr601.h"
#include "img/colorModelConverter.h"

PixelYCbCr601::PixelYCbCr601(unsigned char y, unsigned char cb, unsigned char cr)
        :
        y(y), cb(cb), cr(cr) {}

PixelType PixelYCbCr601::getPixelType() const {
    return YCbCr601;
}

rgba PixelYCbCr601::toRGBA() const {
    return toRGBA8(this).toRGBA();
}
