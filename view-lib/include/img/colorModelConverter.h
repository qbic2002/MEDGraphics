//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_COLORMODELCONVERTER_H
#define MEDGRAPHICS_COLORMODELCONVERTER_H

#include "PixelHSL.h"
#include "PixelRGB8.h"
#include "PixelRGBA8.h"

PixelHSL toHSL8(const Pixel* pixel);

PixelRGBA8 toRGBA8(const Pixel* pixel);

#endif //MEDGRAPHICS_COLORMODELCONVERTER_H
