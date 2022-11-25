//
// Created by danil on 25.11.2022.
//

#ifndef MEDGRAPHICS_MODERN_COLOR_MODEL_H
#define MEDGRAPHICS_MODERN_COLOR_MODEL_H

#include <stdexcept>
#include <utility>
#include <vector>
#include <memory>
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
    ModernRaster(int width, int height, const std::shared_ptr<float[]>& data, const ColorModel* colorModel)
            : width(width), height(height), data(data), colorModel(colorModel) {
        rgbaData = std::shared_ptr<rgba[]>(new rgba[width * height]);
        fillRgbaData();
    }

    ModernRaster(int width, int height, const std::shared_ptr<float[]>& data, const ColorModelEnum colorModelEnum)
            : ModernRaster(width, height, data, findColorModel(colorModelEnum)) {}

    ModernRaster(const ModernRaster& other) : width(other.width), height(other.height), colorModel(other.colorModel) {
        int length = width * height * other.colorModel->getComponentsCount();
        data = std::shared_ptr<float[]>(new float[length]);
        for (int i = 0; i < length; i++) {
            data[i] = other.data[i];
        }
        rgbaData = std::shared_ptr<rgba[]>(new rgba[width * height]);
        fillRgbaData();
    }

    ModernRaster(const ModernRaster&& other) noexcept: ModernRaster(other.width, other.height, other.data,
                                                                    other.colorModel) {
        rgbaData = other.rgbaData;
    }

    int getWidth() const override {
        return width;
    }

    int getHeight() const override {
        return height;
    }

    const unsigned char* getRgbaData() const override {
        return (unsigned char*) rgbaData.get();
    }

    PixelType getPixelType() const override {
        throw std::runtime_error("(");
    }

    ModernRaster* clone() const override {
        return new ModernRaster(*this);
    }

    const ColorModel* getColorModel() const {
        return colorModel;
    }

    const float* getData() const {
        return data.get();
    }

    void reinterpretColorModel(const ColorModelEnum colorModelEnum) {
        if (this->colorModel->getEnum() == colorModelEnum)
            return;
        auto colorModel = findColorModel(colorModelEnum);
        if (this->colorModel->getComponentsCount() != colorModel->getComponentsCount())
            throw std::runtime_error("color model components count must be the same");

        this->colorModel = colorModel;
        fillRgbaData();
    }

    void convertToColorModel(const ColorModelEnum colorModelEnum) {
        if (colorModel->getEnum() == colorModelEnum)
            return;
        auto destColorModel = findColorModel(colorModelEnum);
        if (colorModel->getComponentsCount() != 3)
            throw std::runtime_error("color model only with 3 components are supported");

        int length = width * height;
        float* dataPtr = data.get();
        int componentsCount = colorModel->getComponentsCount();
        for (int i = 0; i < length; i++) {
            auto rgbaf = colorModel->toRgba(dataPtr);
            destColorModel->fromRgb(rgbaf.r, rgbaf.g, rgbaf.b, dataPtr);
            dataPtr += componentsCount;
        }

        colorModel = destColorModel;
        fillRgbaData();
    }

    void setFilter(int index, bool value) {
        if (filter[index] == value)
            return;
        filter[index] = value;
        fillRgbaData();
    }

    bool getFilter(int index) {
        return filter[index];
    }

private:
    void fillRgbaData() {
        int length = width * height;
        const float* dataPtr = data.get();
        int componentsCount = colorModel->getComponentsCount();
        float components[4];
        for (int i = 0; i < length; i++) {
            for (int c = 0; c < componentsCount; c++)
                components[c] = filter[c] ? dataPtr[c] : 0;
            rgbaData[i] = colorModel->toRgba(components).toRgba();
            dataPtr += componentsCount;
        }
    }

    int width = 0;
    int height = 0;
    std::shared_ptr<float[]> data;
    std::shared_ptr<rgba[]> rgbaData;
    const ColorModel* colorModel = findColorModel(COLOR_MODEL_RGB);
    bool filter[4] = {true, true, true, true};
};

#endif //MEDGRAPHICS_MODERN_COLOR_MODEL_H
