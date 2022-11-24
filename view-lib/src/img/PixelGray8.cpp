//
// Created by golov on 24.11.2022.
//

#include "img/PixelType.h"
#include "img/PixelGray8.h"
#include "img/colorModelConverter.h"

PixelType PixelGray8::getPixelType() const {
    return GRAY8;
}

rgba PixelGray8::toRGBA() const {
    return toRGB8(this).toRGBA();
}
