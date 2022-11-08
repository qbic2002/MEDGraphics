//
// Created by danil on 08.11.2022.
//

#include "PNMImage.h"

PNMImage::PNMImage(const AbstractRaster* abstractRaster) {
    pnmHeader.height = abstractRaster->getHeight();
    pnmHeader.width = abstractRaster->getWidth();
    pnmHeader.maxGray = 255;
    switch (abstractRaster->getPixelType()) {
        case PixelType::GRAY: {
            pnmHeader.pnmMode = PNMMode::P5;
            break;
        }
        case PixelType::RGBA8:
        case PixelType::RGB8: {
            pnmHeader.pnmMode = PNMMode::P6;
            break;
        }
    }
    data = abstractRaster->clone();
}

PNMImage::PNMImage(const PNMImage& other) : pnmHeader(other.pnmHeader), pnmMeta(other.pnmMeta) {
    if (other.data)
        data = other.data->clone();
}

PNMImage::PNMImage(PNMImage&& other) noexcept: pnmHeader(other.pnmHeader), pnmMeta(other.pnmMeta) {
    data = other.data;
    other.data = nullptr;
}

PNMImage::~PNMImage() {
    delete data;
}

void PNMMeta::addMeta(const std::string& meta, int offset) {
    meta_.emplace_back(meta, offset);
}

const std::vector<std::pair<std::string, int>>& PNMMeta::getMeta() const {
    return meta_;
}
