//
// Created by golov on 24.11.2022.
//

#include "img/PixelType.h"
#include "img/PixelRGB8.h"
#include "img/colorModelConverter.h"

PixelType PixelRGB8::getPixelType() const {
    return RGB8;
}

rgba PixelRGB8::toRGBA() const {
    return toRGBA8(this).toRGBA();
}

PixelRGB8::PixelRGB8(unsigned char r, unsigned char g, unsigned char b)
        :
        r(r), g(g), b(b) {}
