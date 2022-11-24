//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_PIXELTYPEMETHODS_H
#define MEDGRAPHICS_PIXELTYPEMETHODS_H

#include "img/PixelHSL.h"
#include "img/PixelRGB8.h"
#include "img/PixelGray8.h"
#include "img/PixelRGBA8.h"
#include "img/PixelType.h"
#include "img/PixelHSV.h"
#include "img/PixelYCbCr601.h"
#include "img/PixelYCbCr709.h"
#include <type_traits>

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
            return GRAY8;
        }
        if (std::is_same<T, PixelHSL>::value) {
            return HSL;
        }
        if (std::is_same<T, PixelHSV>::value) {
            return HSV;
        }
        if (std::is_same<T, PixelYCbCr601>::value) {
            return YCbCr601;
        }
        if (std::is_same<T, PixelYCbCr709>::value) {
            return YCbCr709;
        }
    }
}

#endif //MEDGRAPHICS_PIXELTYPEMETHODS_H
