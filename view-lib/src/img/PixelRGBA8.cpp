//
// Created by golov on 24.11.2022.
//

#include "img/PixelType.h"
#include "img/PixelRGBA8.h"
#include "img/colorModelConverter.h"

PixelType PixelRGBA8::getPixelType() const {
    return RGBA8;
}

rgba PixelRGBA8::toRGBA() const {
    return toRGB8(this).toRGBA();
}
