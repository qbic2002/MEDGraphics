//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGELOADER_H
#define MEDGRAPHICS_IMAGELOADER_H

#include <string>
#include <vector>
#include "AbstractRaster.h"

namespace img {
    AbstractRaster* loadImageData(const std::string& supported);

    AbstractRaster* loadImageData(const std::vector<char>& bytes);
}

#endif //MEDGRAPHICS_IMAGELOADER_H
