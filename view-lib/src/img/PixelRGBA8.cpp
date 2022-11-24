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
    return rgba;
}

PixelRGBA8::PixelRGBA8(struct rgba rgba)
        :
        rgba(rgba) {
}

PixelRGBA8::PixelRGBA8(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        :
        r(r), g(g), b(b), a(a) {
}

PixelRGBA8::PixelRGBA8()
        :
        PixelRGBA8(0, 0, 0, 0) {}
