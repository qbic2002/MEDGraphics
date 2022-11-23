//
// Created by golov on 17.09.2022.
//

#ifndef MEDGRAPHICS_PIXEL_H
#define MEDGRAPHICS_PIXEL_H

#include "rgba.h"
#include "PixelType.h"


class Pixel {
public:
    virtual rgba toRGBA() const = 0;

    virtual PixelType getPixelType() const = 0;

    virtual ~Pixel() = default;
};


#endif //MEDGRAPHICS_PIXEL_H
