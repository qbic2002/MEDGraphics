//
// Created by golov on 23.11.2022.
//

#ifndef MEDGRAPHICS_PIXELHSL8_H
#define MEDGRAPHICS_PIXELHSL8_H


#include "Pixel.h"
#include <cmath>
#include <iostream>

class PixelHSL8 : public Pixel {
public:
    PixelHSL8() = default;

    PixelHSL8(const PixelHSL8& other) = default;

    PixelHSL8(double h, double s, double l)
            :
            h(h), s(s), l(l) {}

    rgba toRGBA() const override {
        double q;
        if (l < 0.5) {
            q = l * (1 + s);
        } else {
            q = l + s - (l * s);
        }

        double p = 2 * l - q;
        double h1 = h / 360;

        double tC[3];
        tC[0] = h1 + 1.0 / 3;
        tC[1] = h1;
        tC[2] = h1 - 1.0 / 3;

        for (auto& color: tC) {
            if (color < 0) color += 1;
            if (color > 1) color -= 1;
        }


        double rgb[3];
        for (int i = 0; i < 3; ++i) {
            if (tC[i] < 1.0 / 6) {
                rgb[i] = p + ((q - p) * 6 * tC[i]);
            } else if (tC[i] < 1.0 / 2) {
                rgb[i] = q;
            } else if (tC[i] < 2.0 / 3) {
                rgb[i] = p + ((q - p) * (2.0 / 3 - tC[i]) * 6);
            } else {
                rgb[i] = p;
            }
        }


        return {(unsigned char) round(rgb[0] * 255), (unsigned char) round(rgb[1] * 255),
                (unsigned char) round(rgb[2] * 255), 255};
    }

    PixelType getPixelType() const override {
        return HSL8;
    }

public:
    double h{};
    double s{};
    double l{};
};


#endif //MEDGRAPHICS_PIXELHSL8_H
