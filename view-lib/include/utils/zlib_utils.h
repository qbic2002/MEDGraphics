//
// Created by golov on 09.12.2022.
//

#ifndef MEDGRAPHICS_ZLIB_UTILS_H
#define MEDGRAPHICS_ZLIB_UTILS_H

#include <vector>

namespace zlib {
    std::vector<unsigned char> inflate(const unsigned char* data, int size);
}

#endif //MEDGRAPHICS_ZLIB_UTILS_H
