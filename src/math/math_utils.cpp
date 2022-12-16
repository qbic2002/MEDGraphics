//
// Created by danil on 16.12.2022.
//

#include "math_utils.h"

int parseIntOrDefault(const String& str, int defaultValue) {
    try {
        return std::stoi(str);
    } catch (const std::exception& e) {
        return defaultValue;
    }
}

float parseFloatOrDefault(const String& str, float defaultValue) {
    try {
        return std::stof(str);
    } catch (const std::exception& e) {
        return defaultValue;
    }
}
