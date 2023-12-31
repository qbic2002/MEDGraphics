//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PIXELRGBA8_H
#define MEDGRAPHICS_PIXELRGBA8_H

#include "Pixel.h"
#include "PixelGray8.h"

class PixelRGBA8 : public Pixel {
public:
    PixelRGBA8();

    PixelRGBA8(const PixelRGBA8& other) = default;

    PixelRGBA8(rgba rgba);

    PixelRGBA8(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    union {
        rgba rgba;
        struct {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
    };
};


#endif //MEDGRAPHICS_PIXELRGBA8_H
