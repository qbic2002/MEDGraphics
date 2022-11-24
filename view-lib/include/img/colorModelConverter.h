//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_COLORMODELCONVERTER_H
#define MEDGRAPHICS_COLORMODELCONVERTER_H

#include "PixelHSL8.h"
#include "PixelRGB8.h"

PixelHSL8 toHSL8(const Pixel* pixel);

PixelRGB8 toRGB8(const Pixel* pixel);

#endif //MEDGRAPHICS_COLORMODELCONVERTER_H
