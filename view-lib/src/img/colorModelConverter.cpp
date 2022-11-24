//
// Created by golov on 23.11.2022.
//

#include <algorithm>
#include "img/colorModelConverter.h"
#include "img/PixelGray8.h"
#include "img/PixelRGBA8.h"

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

PixelRGB8 toRGB8(const Pixel* pixel) {
    switch (pixel->getPixelType()) {
        case GRAY8: {
            PixelGray8 p = *(PixelGray8*) pixel;
            return {p.grayScale, p.grayScale, p.grayScale};
        }
        case RGBA8: {
            PixelRGBA8 p = *(PixelRGBA8*) pixel;
            return {p.r, p.g, p.b};
        }
        case RGB8: {
            PixelRGB8 p = *(PixelRGB8*) pixel;
            return p;
        }
        case HSL8: {
            PixelHSL8 pix = *(PixelHSL8*) pixel;
            double l = pix.l;
            double h = pix.h;
            double s = pix.s;

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
                    (unsigned char) round(rgb[2] * 255)};
        }
        default:
            throw std::exception();
    }
}
