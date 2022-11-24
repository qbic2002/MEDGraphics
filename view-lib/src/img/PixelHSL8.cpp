//
// Created by golov on 24.11.2022.
//

#include "img/PixelHSL8.h"
#include "img/PixelType.h"
#include "img/colorModelConverter.h"

PixelType PixelHSL8::getPixelType() const {
    return HSL8;
}

rgba PixelHSL8::toRGBA() const {
    return toRGB8(this).toRGBA();
}
