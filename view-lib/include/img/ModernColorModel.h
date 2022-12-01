//
// Created by danil on 25.11.2022.
//

#ifndef MEDGRAPHICS_MODERN_COLOR_MODEL_H
#define MEDGRAPHICS_MODERN_COLOR_MODEL_H

#include <stdexcept>
#include <utility>
#include <vector>
#include <memory>
#include <cmath>

#include "AbstractRaster.h"
#include "rgba.h"
#include "ColorModels.h"

//class Pixel1f {
//public:
//    static Pixel1f fromInteger(int value, int scale) {
//        return {value / (float) scale};
//    }
//
//    float a;
//};
//
//class Pixel3f {
//public:
//    static Pixel3f from255(unsigned char a, unsigned char b, unsigned char c) {
//        return {a / 255.0f, b / 255.0f, c / 255.0f};
//    }
//
//    union {
//        struct {
//            float a = 0, b = 0, c = 0;
//        };
//        float v[3];
//    };
//};
//
//class Pixel4f {
//public:
//    static Pixel4f from255(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
//        return {a / 255.0f, b / 255.0f, c / 255.0f, d / 255.0f};
//    }
//
//    Pixel3f drop4Component() {
//        return {a, b, c};
//    }
//
//    union {
//        struct {
//            float a = 0, b = 0, c = 0, d = 0;
//        };
//        float v[4];
//    };
//};

class ModernRaster : public AbstractRaster {
public:
    ModernRaster(int width, int height, const std::shared_ptr<float[]>& data, const ColorModel* colorModel);

    ModernRaster(int width, int height, const std::shared_ptr<float[]>& data, const ColorModelEnum colorModelEnum);

    ModernRaster(const ModernRaster& other);

    ModernRaster(const ModernRaster&& other) noexcept;

    int getWidth() const override;

    int getHeight() const override;

    const unsigned char* getRgbaData() const override;

    PixelType getPixelType() const override;

    ModernRaster* clone() const override;

    const ColorModel* getColorModel() const;

    const float* getData() const;

    void reinterpretColorModel(const ColorModelEnum colorModelEnum);

    void reinterpretGamma(float gamma);

    void convertToNewGamma(float gamma);

    void convertToColorModel(const ColorModelEnum colorModelEnum);

    void setFilter(int index, bool value);

    bool getFilter(int index) const;

    float getGamma() const {
        return gamma;
    };

private:
    void fillRgbaData();

    int width = 0;
    int height = 0;
    std::shared_ptr<float[]> data;
    std::shared_ptr<rgba[]> rgbaData;
    const ColorModel* colorModel = findColorModel(COLOR_MODEL_RGB);
    bool filter[4] = {true, true, true, true};
    float gamma = 1;
};

#endif //MEDGRAPHICS_MODERN_COLOR_MODEL_H
