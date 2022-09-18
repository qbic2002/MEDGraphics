//
// Created by golov on 18.09.2022.
//

#ifndef MEDGRAPHICS_PIXELTYPE_H
#define MEDGRAPHICS_PIXELTYPE_H

#include <type_traits>
#include "RGBAPixel.h"
#include "GrayPixel.h"

enum PixelType {
    RGBA,
    GRAY
};

namespace utils {
    template<class T>
    PixelType getPixelType() {
        if (std::is_same<T, RGBAPixel>::value) {
            return RGBA;
        }
        if (std::is_same<T, GrayPixel>::value) {
            return GRAY;
        }
    }
}

#endif //MEDGRAPHICS_PIXELTYPE_H
