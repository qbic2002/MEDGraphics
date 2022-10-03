//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGELOADER_H
#define MEDGRAPHICS_IMAGELOADER_H

#include <string>
#include <vector>
#include "AbstractRaster.h"

namespace img {
    typedef std::basic_string<unsigned char> ustring;

    AbstractRaster* loadImageData(const std::string& supported);

    AbstractRaster* loadImageData(const std::vector<char>& bytes);

    bool isImage(const std::string& fileName);
}

#endif //MEDGRAPHICS_IMAGELOADER_H
