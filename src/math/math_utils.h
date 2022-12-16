//
// Created by danil on 16.12.2022.
//

#ifndef MEDGRAPHICS_MATH_UTILS_H
#define MEDGRAPHICS_MATH_UTILS_H

#include "utils/String.h"

int parseIntOrDefault(const String& str, int defaultValue);

float parseFloatOrDefault(const String& str, float defaultValue);

template<typename TNumber>
inline TNumber clamp(TNumber value, TNumber min, TNumber max) {
    if (value < min)
        return min;
    if (max < value)
        return max;
    return value;
}

#endif //MEDGRAPHICS_MATH_UTILS_H
