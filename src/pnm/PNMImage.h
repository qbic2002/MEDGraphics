//
// Created by golov on 15.09.2022.
//

#ifndef CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
#define CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H


#include "PNMHeader.h"
#include "../img/Raster.h"
#include "PNMMeta.h"
#include "../img/RGBAPixel.h"

class PNMImage {
public:
    PNMImage() = default;

    PNMImage(const PNMImage& other) = default;

    PNMImage& operator=(const PNMImage& other) = default;

    PNMHeader pnmHeader;
    PNMMeta pnmMeta;
    Raster<RGBAPixel> rgbaData;
};


#endif //CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
