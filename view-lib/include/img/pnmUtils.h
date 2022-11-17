//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNM_UTILS_H
#define MEDGRAPHICS_PNM_UTILS_H

#include <filesystem>
#include "PNMImage.h"

namespace pnm {
    int parsePnmHeader(const char* fileData, PNMHeader& pnmHeader);

    AbstractRaster*
    parseData(const unsigned char* fileData, int offset, const PNMHeader& pnmHeader, unsigned int length);

    PNMMeta parseMeta(const char* fileData, int headerSize);

    PNMImage readPnmImage(const std::filesystem::path& filename);

    void writePNMImage(const PNMImage& pnmImage, std::ofstream& os);

    void writePNMImage(const PNMImage& pnmImage, const std::filesystem::path& filename);

    PNMImage convertP6ToP5(const PNMImage& other);

    PNMImage readPNMImageFromMemory(const char* data, unsigned int length);
}


#endif //MEDGRAPHICS_PNM_UTILS_H
