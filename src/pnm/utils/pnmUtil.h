//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNMUTIL_H
#define MEDGRAPHICS_PNMUTIL_H

#include "../PNMImage.h"
#include "../PNMMeta.h"

namespace pnm {
    int parsePnmHeader(const char* fileData, PNMHeader& pnmHeader);

    AbstractRaster*
    parseData(const unsigned char* fileData, int offset, const PNMHeader& pnmHeader, unsigned int length);

    PNMMeta parseMeta(const char* fileData, int headerSize);

    PNMImage readPNMImage(const char* fileName);

    bool writePNMImage(const PNMImage& pnmImage, const char* filename);

    PNMImage convertP6ToP5(const PNMImage& other);

    PNMImage readPNMImageFromMemory(const char* data, unsigned int length);
}


#endif //MEDGRAPHICS_PNMUTIL_H
