//
// Created by danil on 08.11.2022.
//

#include "img/PNMImage.h"

PNMImage::PNMImage(const PNMHeader& header, const PNMMeta& meta, const ModernRaster& raster)
        : header(header), meta(meta), raster(raster) {}

PNMImage::PNMImage(const ModernRaster& raster) : raster(raster) {
    header.height = raster.getHeight();
    header.width = raster.getWidth();
    header.maxValue = 255;
    switch (raster.getColorModel()->getComponentsCount()) {
        case 1:
            header.mode = P5;
            break;
        case 3:
            header.mode = P6;
            break;
        default:
            throw std::runtime_error("PNM can store only 1 component or 3");
    }
}

PNMImage::PNMImage(const PNMImage& other) = default;

void PNMMeta::addMeta(const std::string& meta, int offset) {
    meta_.emplace_back(meta, offset);
}

const std::vector<std::pair<std::string, int>>& PNMMeta::getMeta() const {
    return meta_;
}
