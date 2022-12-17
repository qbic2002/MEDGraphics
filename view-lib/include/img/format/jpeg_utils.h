//
// Created by danil on 16.12.2022.
//

#ifndef MEDGRAPHICS_JPEG_UTILS_H
#define MEDGRAPHICS_JPEG_UTILS_H

#include "img/ModernColorModel.h"

namespace img {
    ModernRaster* readJpegImage(const char* data, int length);
}

#endif //MEDGRAPHICS_JPEG_UTILS_H
