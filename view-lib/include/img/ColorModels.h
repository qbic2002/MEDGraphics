//
// Created by danil on 25.11.2022.
//

#ifndef MEDGRAPHICS_COLOR_MODELS_H
#define MEDGRAPHICS_COLOR_MODELS_H

#include "rgba.h"
#include "algorithm"

struct rgbaF {
    float r, g, b, a;

    rgba toRgba() const {
        return to255rgba(r, g, b, a);
    }
};

enum ColorModelEnum {
    COLOR_MODEL_RGB = 0,
    COLOR_MODEL_RGBA = 1,
    COLOR_MODEL_GRAY = 2,
    COLOR_MODEL_HSL = 3
};

class ColorModel {
public:
    ColorModel() = default;

    ColorModel(ColorModel& other) = delete;

    ColorModel(ColorModel&& other) = delete;

    virtual rgbaF toRgba(const float* data) const = 0;

    virtual void fromRgb(float r, float g, float b, float* dest) const = 0;

    virtual int getComponentsCount() const = 0;

    virtual ColorModelEnum getEnum() const = 0;
};

const ColorModel* findColorModel(ColorModelEnum colorModelEnum);

class ColorModelGray : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override {
        return {data[0], data[0], data[0], 1};
    }

    void fromRgb(float r, float g, float b, float* dest) const override {
        dest[0] = (r + g + b) / 3;
    }

    int getComponentsCount() const override {
        return 1;
    }

    ColorModelEnum getEnum() const override {
        return COLOR_MODEL_GRAY;
    }
};

class ColorModelRGB : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override {
        return {data[0], data[1], data[2], 1};
    }

    void fromRgb(float r, float g, float b, float* dest) const override {
        dest[0] = r;
        dest[1] = g;
        dest[2] = b;
    }

    int getComponentsCount() const override {
        return 3;
    }

    ColorModelEnum getEnum() const override {
        return COLOR_MODEL_RGB;
    }
};

class ColorModelRGBA : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override {
        return {data[0], data[1], data[2], data[3]};
    }

    void fromRgb(float r, float g, float b, float* dest) const override {
        dest[0] = r;
        dest[1] = g;
        dest[2] = b;
        dest[3] = 1;
    }

    int getComponentsCount() const override {
        return 4;
    }

    ColorModelEnum getEnum() const override {
        return COLOR_MODEL_RGBA;
    }
};

class ColorModelHSL : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override {
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

    void fromRgb(float r, float g, float b, float* dest) const override {
        r *= 255;
        g *= 255;
        b *= 255;

        float cMax = std::max(std::max(r, g), b);
        float cMin = std::min(std::min(r, g), b);

        float delta = cMax - cMin;

        float h = 0;
        if (delta == 0) {
            h = 0;
        } else if (cMax == r) {
            h = 60 * (((g - b) / delta));
            if (g < b) {
                h += 360;
            }
        } else if (cMax == g) {
            h = 60.0 * (((b - r) / delta) + 2.0);
        } else if (cMax == b) {
            h = 60 * (((r - g) / delta) + 4.0);
        }

        float l = (cMax + cMin) / 255.0 / 2.0;
        float s = 0;
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

    int getComponentsCount() const override {
        return 3;
    }

    ColorModelEnum getEnum() const override {
        return COLOR_MODEL_HSL;
    }
};

#endif //MEDGRAPHICS_COLOR_MODELS_H

