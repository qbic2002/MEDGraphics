//
// Created by golov on 08.12.2022.
//

#ifndef MEDGRAPHICS_PNGUTILS_H
#define MEDGRAPHICS_PNGUTILS_H

#include "PNGImage.h"

namespace png {
    PNGImage readPNGImageFromMemory(const char* data, unsigned int length);
}

#endif //MEDGRAPHICS_PNGUTILS_H
