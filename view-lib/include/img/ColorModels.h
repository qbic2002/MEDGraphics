//
// Created by danil on 25.11.2022.
//

#ifndef MEDGRAPHICS_COLOR_MODELS_H
#define MEDGRAPHICS_COLOR_MODELS_H

#include "rgba.h"
#include "algorithm"

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

struct rgbaF {
    union {
        struct {
            float r, g, b, a;
        };
        float components[4];
    };

    rgba toRgba() const;
};

enum ColorModelEnum {
    COLOR_MODEL_RGB = 0,
    COLOR_MODEL_RGBA = 1,
    COLOR_MODEL_GRAY = 2,
    COLOR_MODEL_HSL = 3,
    COLOR_MODEL_HSV = 4,
    COLOR_MODEL_YCbCr601 = 5,
    COLOR_MODEL_YCbCr709 = 6,
    COLOR_MODEL_YCoCg = 7,
    COLOR_MODEL_CMY = 8
};

class ColorModel {
public:
    ColorModel() = default;

    ColorModel(ColorModel& other) = delete;

    ColorModel(ColorModel&& other) = delete;

    virtual rgbaF toRgba(const float* data) const = 0;

    virtual void fromRgba(rgbaF color, float* dest) const = 0;

    virtual int getComponentsCount() const = 0;

    virtual ColorModelEnum getEnum() const = 0;
};

const ColorModel* findColorModel(ColorModelEnum colorModelEnum);

class ColorModelGray : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelRGB : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelRGBA : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelHSL : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelHSV : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelYCbCr601 : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelYCbCr709 : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelYCoCg : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

class ColorModelCMY : public ColorModel {
public:
    rgbaF toRgba(const float* data) const override;

    void fromRgba(rgbaF color, float* dest) const override;

    int getComponentsCount() const override;

    ColorModelEnum getEnum() const override;
};

#endif //MEDGRAPHICS_COLOR_MODELS_H

