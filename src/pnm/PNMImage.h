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

    PNMImage(const AbstractRaster* abstractRaster) {
        pnmHeader.height = abstractRaster->getHeight();
        pnmHeader.width = abstractRaster->getWidth();
        pnmHeader.maxGrey = 255;
        PixelType pixelType = abstractRaster->getPixelType();
        switch (pixelType) {
            case PixelType::GRAY: {
                pnmHeader.pnmMode = PNMMode::P5;

                Raster<GrayPixel>* raster = (Raster<GrayPixel>*) abstractRaster;
                data = new Raster<GrayPixel>(*raster);
                break;
            }
            case PixelType::RGBA: {
                pnmHeader.pnmMode = PNMMode::P6;

                Raster<RGBAPixel>* raster = (Raster<RGBAPixel>*) abstractRaster;
                data = new Raster<RGBAPixel>(*raster);
                break;
            }
        }
    }

    PNMImage(const PNMImage& other) = delete;

    PNMImage(PNMImage&& other) noexcept: pnmHeader(other.pnmHeader), pnmMeta(other.pnmMeta) {
        data = other.data;
        other.data = nullptr;
    }

    PNMImage& operator=(const PNMImage& other) = delete;


    ~PNMImage() {
        delete data;
    }

    PNMHeader pnmHeader;
    PNMMeta pnmMeta;
    AbstractRaster* data = nullptr;
};


#endif //CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
