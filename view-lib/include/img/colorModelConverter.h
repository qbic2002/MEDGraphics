//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_COLORMODELCONVERTER_H
#define MEDGRAPHICS_COLORMODELCONVERTER_H

#include "PixelHSL.h"
#include "PixelRGB8.h"
#include "PixelRGBA8.h"
#include "PixelHSV.h"
#include "PixelYCbCr601.h"
#include "PixelYCbCr709.h"
#include "PixelYCoCg.h"

PixelHSL toHSL(const Pixel* pixel);

PixelHSV toHSV(const Pixel* pixel);

PixelYCbCr601 toYCbCr601(const Pixel* pixel);

PixelYCbCr709 toYCbCr709(const Pixel* pixel);

PixelYCoCg toYCoCg(const Pixel* pixel);

PixelRGBA8 toRGBA8(const Pixel* pixel);

#endif //MEDGRAPHICS_COLORMODELCONVERTER_H
