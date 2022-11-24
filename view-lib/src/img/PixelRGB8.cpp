//
// Created by golov on 24.11.2022.
//

#include "img/PixelType.h"
#include "img/PixelRGB8.h"

PixelType PixelRGB8::getPixelType() const {
    return RGB8;
}

rgba PixelRGB8::toRGBA() const {
    return {r, g, b, 255};
}
