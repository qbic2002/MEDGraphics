//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNMUTIL_H
#define MEDGRAPHICS_PNMUTIL_H

#include "../PNMImage.h"

namespace pnm {
    int parsePnmHeader(const char* fileData, PNMHeader& pnmHeader);
    RGBAData parseData(const unsigned char* fileData, int offset, const PNMHeader& pnmHeader);

    PNMImage readPNMImage(const char* fileName);
}


#endif //MEDGRAPHICS_PNMUTIL_H
