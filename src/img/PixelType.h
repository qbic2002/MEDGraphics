//
// Created by golov on 18.09.2022.
//

#ifndef MEDGRAPHICS_PIXELTYPE_H
#define MEDGRAPHICS_PIXELTYPE_H

#include <type_traits>
#include "PixelRGBA8.h"
#include "PixelGray8.h"
#include "PixelRGB8.h"

enum PixelType {
    RGBA8,
    RGB8,
    GRAY
};

namespace utils {
    template<class T>
    PixelType getPixelType() {
        if (std::is_same<T, PixelRGBA8>::value) {
            return RGBA8;
        }
        if (std::is_same<T, PixelRGB8>::value) {
            return RGB8;
        }
        if (std::is_same<T, PixelGray8>::value) {
            return GRAY;
        }
    }
}

#endif //MEDGRAPHICS_PIXELTYPE_H
