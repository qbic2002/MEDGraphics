//
// Created by golov on 01.12.2022.
//

#ifndef MEDGRAPHICS_DITHERINGMETHODS_H
#define MEDGRAPHICS_DITHERINGMETHODS_H

#include "rgba.h"

enum DitheringMethods {
    ORDERED_DITHERING = 0,
    RANDOM_DITHERING = 1,
    FLOYD_STEINBERG_DITHERING = 2,
    ATKINSON_DITHERING = 3
};

class DitheringMethod {
public:
    virtual void dither(int bits, const rgba* data, rgba* dest, int height, int width) const = 0;
};

const DitheringMethod* findDitheringMethod(DitheringMethods ditheringMethod);

class OrderedDithering : public DitheringMethod {
public:
    void dither(int bits, const rgba* data, rgba* dest, int height, int width) const override;
};

class RandomDithering : public DitheringMethod {
public:
    void dither(int bits, const rgba* data, rgba* dest, int height, int width) const override;
};

class FloydSteinbergDithering : public DitheringMethod {
public:
    void dither(int bits, const rgba* data, rgba* dest, int height, int width) const override;
};

class AtkinsonDithering : public DitheringMethod {
public:
    void dither(int bits, const rgba* data, rgba* dest, int height, int width) const override;
};

#endif //MEDGRAPHICS_DITHERINGMETHODS_H
