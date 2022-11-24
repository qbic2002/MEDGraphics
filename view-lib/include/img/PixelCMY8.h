//
// Created by golov on 24.11.2022.
//

#ifndef MEDGRAPHICS_PIXELCMY8_H
#define MEDGRAPHICS_PIXELCMY8_H


#include "Pixel.h"

class PixelCMY8 : public Pixel {
public:
    PixelCMY8() = default;

    PixelCMY8(const PixelCMY8& other) = default;

    PixelCMY8(unsigned char c, unsigned char m, unsigned char y);

    rgba toRGBA() const override;

    PixelType getPixelType() const override;

public:
    unsigned char c{};
    unsigned char m{};
    unsigned char y{};
};


#endif //MEDGRAPHICS_PIXELCMY8_H
