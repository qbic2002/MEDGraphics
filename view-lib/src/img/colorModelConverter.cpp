//
// Created by golov on 23.11.2022.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include "img/colorModelConverter.h"
#include "img/PixelGray8.h"
#include "img/PixelRGBA8.h"
#include "img/PixelHSV.h"
#include "img/ModernColorModel.h"

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

PixelHSL toHSL(const Pixel* pixel) {
    if (pixel->getPixelType() == HSL) {
        return *(PixelHSL*) pixel;
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

PixelHSV toHSV(const Pixel* pixel) {
    if (pixel->getPixelType() == HSV) {
        return *(PixelHSV*) pixel;
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

    double s = 0;
    if (cMax == 0) {
        s = 0;
    } else {
        s = 1 - (double) cMin / cMax;
    }

    double v = cMax / 255.0;
    return {h, s, v};
}

PixelYCbCr601 toYCbCr601(const Pixel* pixel) {
    if (pixel->getPixelType() == YCbCr601) {
        return *(PixelYCbCr601*) pixel;
    }

    rgba rgba = pixel->toRGBA();
    double y = (16 + (65.481 * rgba.r / 255.0) + (128.553 * rgba.g / 255.0) + (24.966 * rgba.b / 255.0));
    double cb = (128 - (37.797 * rgba.r / 255.0) - (74.203 * rgba.g / 255.0) + (112.0 * rgba.b / 255.0));
    double cr = (128 + (112.0 * rgba.r / 255.0) - (93.786 * rgba.g / 255.0) - (18.214 * rgba.b / 255.0));

    return {y, cb, cr};

}

PixelYCbCr709 toYCbCr709(const Pixel* pixel) {
    if (pixel->getPixelType() == YCbCr709) {
        return *(PixelYCbCr709*) pixel;
    }

    rgba rgba = pixel->toRGBA();
    double y = (0 + (0.299 * rgba.r) + (0.587 * rgba.g) + (0.114 * rgba.b));
    double cb = (128 - (0.168736 * rgba.r) - (0.331264 * rgba.g) + (0.5 * rgba.b));
    double cr = (128 + (0.5 * rgba.r) - (0.418688 * rgba.g) - (0.081312 * rgba.b));

    return {y, cb, cr};

}

PixelYCoCg toYCoCg(const Pixel* pixel) {
    if (pixel->getPixelType() == YCoCg) {
        return *(PixelYCoCg*) pixel;
    }

    rgba rgba = pixel->toRGBA();
    double y = ((0.25 * rgba.r) + (0.5 * rgba.g) + (0.25 * rgba.b));
    double co = ((0.5 * rgba.r) - (0.5 * rgba.b));
    double cg = (-(0.25 * rgba.r) + (0.5 * rgba.g) - (0.25 * rgba.b));

    return {y, co, cg};
}

PixelCMY8 toCMY8(const Pixel* pixel) {
    if (pixel->getPixelType() == CMY8) {
        return *(PixelCMY8*) pixel;
    }

    rgba rgba = pixel->toRGBA();

    unsigned char c = (255 - rgba.r);
    unsigned char m = (255 - rgba.g);
    unsigned char y = (255 - rgba.b);
    return {c, m, y};
}

PixelRGBA8 toRGBA8(const Pixel* pixel) {
    switch (pixel->getPixelType()) {
        case GRAY8: {
            PixelGray8 p = *(PixelGray8*) pixel;
            return {p.grayScale, p.grayScale, p.grayScale, 255};
        }
        case RGBA8: {
            PixelRGBA8 p = *(PixelRGBA8*) pixel;
            return p;
        }
        case RGB8: {
            PixelRGB8 p = *(PixelRGB8*) pixel;
            return {p.r, p.g, p.b, 255};
        }
        case HSL: {
            PixelHSL pix = *(PixelHSL*) pixel;
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
                    (unsigned char) round(rgb[2] * 255), 255};
        }
        case HSV: {
            PixelHSV pix = *(PixelHSV*) pixel;
            char h1 = ((int) pix.h / 60) % 6;
            double vMin = (1 - pix.s) * pix.v;

            double a = (pix.v - vMin) * ((int) pix.h % 60) / 60.0;
            double vInc = vMin + a;
            double vDec = pix.v - a;
            switch (h1) {
                case 0:
                    return {(unsigned char) round(pix.v * 255), (unsigned char) round(vInc * 255),
                            (unsigned char) round(vMin * 255), 255};
                case 1:
                    return {(unsigned char) round(vDec * 255), (unsigned char) round(pix.v * 255),
                            (unsigned char) round(vMin * 255), 255};
                case 2:
                    return {(unsigned char) round(vMin * 255), (unsigned char) round(pix.v * 255),
                            (unsigned char) round(vInc * 255), 255};
                case 3:
                    return {(unsigned char) round(vMin * 255), (unsigned char) round(vDec * 255),
                            (unsigned char) round(pix.v * 255), 255};
                case 4:
                    return {(unsigned char) round(vInc * 255), (unsigned char) round(vMin * 255),
                            (unsigned char) round(pix.v * 255), 255};
                case 5:
                    return {(unsigned char) round(pix.v * 255), (unsigned char) round(vMin * 255),
                            (unsigned char) round(vDec * 255), 255};
            }
        }
        case YCbCr601: {
            PixelYCbCr601 pix = *(PixelYCbCr601*) pixel;
            int r = round((298.082 * pix.y) / 256.0 + (408.583 * pix.cr) / 256.0 - 222.921);
            int g = round((298.082 * pix.y) / 256.0 - (100.291 * pix.cb) / 256.0 - (208.120 * pix.cr) / 256.0 +
                          135.576);
            int b = round((298.082 * pix.y) / 256.0 + (516.412 * pix.cb) / 256.0 - 276.836);

            if (r < 0) r = 0;
            if (r > 255) r = 255;
            if (g < 0) g = 0;
            if (g > 255) g = 255;
            if (b < 0) b = 0;
            if (b > 255) b = 255;

            return {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
        }
        case YCbCr709: {
            PixelYCbCr709 pix = *(PixelYCbCr709*) pixel;
            int r = round((pix.y) + 1.402 * (pix.cr - 128));
            int g = round((pix.y) - 0.344136 * (pix.cb - 128) - 0.714136 * (pix.cr - 128));
            int b = round(pix.y) + 1.772 * (pix.cb - 128);

            if (r < 0) r = 0;
            if (r > 255) r = 255;
            if (g < 0) g = 0;
            if (g > 255) g = 255;
            if (b < 0) b = 0;
            if (b > 255) b = 255;

            return {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
        }
        case YCoCg: {
            PixelYCoCg pix = *(PixelYCoCg*) pixel;
            int g = pix.y + pix.cg;
            double tmp = pix.y - pix.cg;
            int r = tmp + pix.co;
            int b = tmp - pix.co;

            if (r < 0) r = 0;
            if (r > 255) r = 255;
            if (g < 0) g = 0;
            if (g > 255) g = 255;
            if (b < 0) b = 0;
            if (b > 255) b = 255;

            return {(unsigned char) r, (unsigned char) g, (unsigned char) b, 255};
        }
        case CMY8: {
            PixelCMY8 pix = *(PixelCMY8*) pixel;
            unsigned char r = (255 - pix.c);
            unsigned char g = (255 - pix.m);
            unsigned char b = (255 - pix.y);

            return {r, g, b, 255};
        }
        default:
            throw std::exception();
    }
}
