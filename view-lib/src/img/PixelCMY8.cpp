//
// Created by golov on 24.11.2022.
//

#include "img/PixelCMY8.h"
#include "img/colorModelConverter.h"

PixelCMY8::PixelCMY8(unsigned char c, unsigned char m, unsigned char y)
        :
        c(c), m(m), y(y) {}

rgba PixelCMY8::toRGBA() const {
    return toRGBA8(this).toRGBA();
}

PixelType PixelCMY8::getPixelType() const {
    return CMY8;
}

