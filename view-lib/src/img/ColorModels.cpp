//
// Created by danil on 25.11.2022.
//

#include "img/ColorModels.h"

ColorModelRGB colorModelRgb;
ColorModelRGBA colorModelRgba;
ColorModelGray colorModelGray;
ColorModelHSL colorModelHsl;
ColorModelHSV colorModelHsv;
ColorModelYCbCr601 colorModelYCbCr601;
ColorModelYCbCr709 colorModelYCbCr709;
ColorModelYCoCg colorModelYCoCg;
ColorModelCMY colorModelCmy;

ColorModel* colorModels[] = {&colorModelRgb, &colorModelRgba, &colorModelGray, &colorModelHsl, &colorModelHsv,
                             &colorModelYCbCr601, &colorModelYCbCr709, &colorModelYCoCg, &colorModelCmy};

const ColorModel* findColorModel(ColorModelEnum colorModelEnum) {
    return colorModels[colorModelEnum];
}

rgba rgbaF::toRgba() const {
    return to255rgba(r, g, b, a);
}

rgbaF ColorModelGray::toRgba(const float* data) const {
    return {data[0], data[0], data[0], 1};
}

void ColorModelGray::fromRgba(rgbaF color, float* dest) const {
    dest[0] = (color.r + color.g + color.b) / 3;
}

int ColorModelGray::getComponentsCount() const {
    return 1;
}

ColorModelEnum ColorModelGray::getEnum() const {
    return COLOR_MODEL_GRAY;
}

rgbaF ColorModelRGB::toRgba(const float* data) const {
    return {data[0], data[1], data[2], 1};
}

void ColorModelRGB::fromRgba(rgbaF color, float* dest) const {
    dest[0] = color.r;
    dest[1] = color.g;
    dest[2] = color.b;
}

int ColorModelRGB::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelRGB::getEnum() const {
    return COLOR_MODEL_RGB;
}

rgbaF ColorModelRGBA::toRgba(const float* data) const {
    return {data[0], data[1], data[2], data[3]};
}

void ColorModelRGBA::fromRgba(rgbaF color, float* dest) const {
    dest[0] = color.r;
    dest[1] = color.g;
    dest[2] = color.b;
    dest[3] = color.a;
}

int ColorModelRGBA::getComponentsCount() const {
    return 4;
}

ColorModelEnum ColorModelRGBA::getEnum() const {
    return COLOR_MODEL_RGBA;
}

rgbaF ColorModelHSL::toRgba(const float* data) const {
    float h = data[0] * 360;
    float s = data[1];
    float l = data[2];

    float q;
    if (l < 0.5) {
        q = l * (1 + s);
    } else {
        q = l + s - (l * s);
    }

    float p = 2 * l - q;
    float h1 = h / 360;

    float tC[3];
    tC[0] = h1 + 1.0 / 3;
    tC[1] = h1;
    tC[2] = h1 - 1.0 / 3;

    for (auto& color: tC) {
        if (color < 0) color += 1;
        if (color > 1) color -= 1;
    }

    float rgb[3];
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

    return {rgb[0], rgb[1], rgb[2], 1};
}

void ColorModelHSL::fromRgba(rgbaF color, float* dest) const {
    color.r *= 255;
    color.g *= 255;
    color.b *= 255;

    float cMax = std::max(std::max(color.r, color.g), color.b);
    float cMin = std::min(std::min(color.r, color.g), color.b);

    float delta = cMax - cMin;

    float h = 0;
    if (delta == 0) {
        h = 0;
    } else if (cMax == color.r) {
        h = 60 * (((color.g - color.b) / delta));
        if (color.g < color.b) {
            h += 360;
        }
    } else if (cMax == color.g) {
        h = 60.0 * (((color.b - color.r) / delta) + 2.0);
    } else if (cMax == color.b) {
        h = 60 * (((color.r - color.g) / delta) + 4.0);
    }

    float l = (cMax + cMin) / 255.0 / 2.0;
    float s;
    if (delta == 0) {
        s = 0;
    }
    if (delta != 0) {
        s = delta / 255.0 / (1 - std::abs(2 * l - 1));
    }

    dest[0] = h / 360;
    dest[1] = s;
    dest[2] = l;
}

int ColorModelHSL::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelHSL::getEnum() const {
    return COLOR_MODEL_HSL;
}

rgbaF ColorModelHSV::toRgba(const float* data) const {
    float h = data[0] * 360;
    float s = data[1];
    float v = data[2];

    char h1 = ((int) h / 60) % 6;
    float vMin = (1 - s) * v;

    float a = (v - vMin) * ((int) h % 60) / 60.0;
    float vInc = vMin + a;
    float vDec = v - a;
    switch (h1) {
        case 0:
            return {v, vInc, vMin, 1};
        case 1:
            return {vDec, v, vMin, 1};
        case 2:
            return {vMin, v, vInc, 1};
        case 3:
            return {vMin, vDec, v, 1};
        case 4:
            return {vInc, vMin, v, 1};
        case 5:
            return {v, vMin, vDec, 1};
        default:
            throw std::runtime_error("impossible");
    }
}

void ColorModelHSV::fromRgba(rgbaF color, float* dest) const {
    float cMax = max(color.r, color.g, color.b);
    float cMin = min(color.r, color.g, color.b);

    float delta = cMax - cMin;

    float h = 0;
    if (delta == 0) {
        h = 0;
    } else if (cMax == color.r) {
        h = 60 * (((color.g - color.b) / delta));
        if (color.g < color.b) {
            h += 360;
        }
    } else if (cMax == color.g) {
        h = 60.0 * (((color.b - color.r) / delta) + 2.0);
    } else if (cMax == color.b) {
        h = 60 * (((color.r - color.g) / delta) + 4.0);
    }

    float s;
    if (cMax == 0) {
        s = 0;
    } else {
        s = 1 - cMin / cMax;
    }

    float v = cMax;

    dest[0] = h / 360;
    dest[1] = s;
    dest[2] = v;
}

int ColorModelHSV::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelHSV::getEnum() const {
    return COLOR_MODEL_HSV;
}

rgbaF ColorModelYCbCr601::toRgba(const float* data) const {
    float y = data[0] * 255;
    float cb = data[1] * 255;
    float cr = data[2] * 255;

    float r = 255 / 219.0 * (y - 16) + 255 / 224.0 * 1.402 * (cr - 128);
    float g = 255 / 219.0 * (y - 16) - 255 / 224.0 * 1.772 * 0.114 / 0.587 * (cb - 128) -
              255 / 224.0 * 1.402 * 0.299 / 0.587 * (cr - 128);
    float b = 255 / 219.0 * (y - 16) + 255 / 224.0 * 1.772 * (cb - 128);

    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;

    return {r / 255, g / 255, b / 255, 1};

}

void ColorModelYCbCr601::fromRgba(rgbaF color, float* dest) const {
    float y = ((65.481 * color.r) + (128.553 * color.g) + (24.966 * color.b));
    float cb = (128 - (37.797 * color.r) - (74.203 * color.g) + (112.0 * color.b));
    float cr = (128 + (112.0 * color.r) - (93.786 * color.g) - (18.214 * color.b));

    dest[0] = y / 255;
    dest[1] = cb / 255;
    dest[2] = cr / 255;
}

int ColorModelYCbCr601::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelYCbCr601::getEnum() const {
    return COLOR_MODEL_YCbCr601;
}

rgbaF ColorModelYCbCr709::toRgba(const float* data) const {
    float y = data[0] * 255;
    float cb = data[1] * 255;
    float cr = data[2] * 255;

    float r = ((y) + 1.402 * (cr - 128));
    float g = ((y) - 0.344136 * (cb - 128) - 0.714136 * (cr - 128));
    float b = (y) + 1.772 * (cb - 128);

    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;

    return {r / 255, g / 255, b / 255, 1};
}

void ColorModelYCbCr709::fromRgba(rgbaF color, float* dest) const {
    color.r *= 255;
    color.g *= 255;
    color.b *= 255;

    float y = (0 + (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b));
    float cb = (128 - (0.168736 * color.r) - (0.331264 * color.g) + (0.5 * color.b));
    float cr = (128 + (0.5 * color.r) - (0.418688 * color.g) - (0.081312 * color.b));

    dest[0] = y / 255;
    dest[1] = cb / 255;
    dest[2] = cr / 255;
}

int ColorModelYCbCr709::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelYCbCr709::getEnum() const {
    return COLOR_MODEL_YCbCr709;
}

rgbaF ColorModelYCoCg::toRgba(const float* data) const {
    float y = data[0];
    float co = data[1] - 0.5;
    float cg = data[2] - 0.5;

    float g = y + cg;
    float tmp = y - cg;
    float r = tmp + co;
    float b = tmp - co;

    return {r, g, b, 1};
}

void ColorModelYCoCg::fromRgba(rgbaF color, float* dest) const {
    float y = ((0.25 * color.r) + (0.5 * color.g) + (0.25 * color.b));
    float co = ((0.5 * color.r) - (0.5 * color.b));
    float cg = (-(0.25 * color.r) + (0.5 * color.g) - (0.25 * color.b));

    dest[0] = y;
    dest[1] = co + 0.5;
    dest[2] = cg + 0.5;
}

int ColorModelYCoCg::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelYCoCg::getEnum() const {
    return COLOR_MODEL_YCoCg;
}

rgbaF ColorModelCMY::toRgba(const float* data) const {
    return {1 - data[0], 1 - data[1], 1 - data[2], 1};
}

void ColorModelCMY::fromRgba(rgbaF color, float* dest) const {
    dest[0] = 1 - color.r;
    dest[1] = 1 - color.g;
    dest[2] = 1 - color.b;
}

int ColorModelCMY::getComponentsCount() const {
    return 3;
}

ColorModelEnum ColorModelCMY::getEnum() const {
    return COLOR_MODEL_CMY;
}
