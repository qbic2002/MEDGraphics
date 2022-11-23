//
// Created by golov on 23.11.2022.
//

#include <algorithm>
#include <iostream>
#include "img/colorModelConverter.h"

template<class T>
T max(T a, T b) {
    return std::max(a, b);
}

template<class T, class... Arguments>
T max(T a, T b, Arguments... args) {
    return max(std::max(a, b), args...);
}


template<class T>
T min(T a, T b) {
    return std::min(a, b);
}

template<class T, class... Arguments>
T min(T a, T b, Arguments... args) {
    return min(std::min(a, b), args...);
}

PixelHSL8 toHSL8(const Pixel* pixel) {
    if (pixel->getPixelType() == HSL8) {
        return *(PixelHSL8*) pixel;
    }

    rgba rgba = pixel->toRGBA();

    int r = rgba.r;
    int g = rgba.g;
    int b = rgba.b;


    unsigned char cMax = max(rgba.r, rgba.g, rgba.b);
    unsigned char cMin = min(rgba.r, rgba.g, rgba.b);

    double delta = cMax - cMin;

    double h = 0;
    if (delta == 0) {
        h = 0;
    } else if (cMax == rgba.r) {
        h = 60 * (((g - b) / delta));
        if (g < b) {
            h += 360;
        }
    } else if (cMax == rgba.g) {
        h = 60.0 * (((b - r) / delta) + 2.0);
    } else if (cMax == rgba.b) {
        h = 60 * (((r - g) / delta) + 4.0);
    }

    double l = (cMax + cMin) / 255.0 / 2.0;
    double s = 0;
    if (delta == 0) {
        s = 0;
    }
    if (delta != 0) {
        s = delta / 255.0 / (1 - std::abs(2 * l - 1));
    }

    return {h, s, l};
}
