//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_PNMUTIL_H
#define MEDGRAPHICS_PNMUTIL_H

#include "../PNMImage.h"

namespace pnm {
    int parsePnmHeader(const char* fileData, PNMImage& pnmImage);
}


#endif //MEDGRAPHICS_PNMUTIL_H
