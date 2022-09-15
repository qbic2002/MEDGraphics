//
// Created by golov on 15.09.2022.
//

#ifndef CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
#define CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H


#include "PNMHeader.h"
#include "../img/RGBAData.h"

class PNMImage {
public:
    PNMImage() = default;
    PNMImage(const PNMImage& other) : pnmHeader(other.pnmHeader), rgbaData(other.rgbaData){}

    PNMImage operator=(const PNMImage& other){
        if (&other == this){
            return *this;
        }

        pnmHeader = other.pnmHeader;
        rgbaData = other.rgbaData;

        return *this;
    }

    PNMHeader pnmHeader;
    RGBAData rgbaData;
};


#endif //CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
