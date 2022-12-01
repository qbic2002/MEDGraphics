//
// Created by golov on 30.11.2022.
//


#include "img/ModernColorModel.h"

ModernRaster::ModernRaster(int width, int height, const std::shared_ptr<float[]>& data, const ColorModel* colorModel)
        : width(width), height(height), data(data), colorModel(colorModel) {
    rgbaData = std::shared_ptr<rgba[]>(new rgba[width * height]);
    fillRgbaData();
}

ModernRaster::ModernRaster(int width, int height, const std::shared_ptr<float[]>& data,
                           const ColorModelEnum colorModelEnum)
        : ModernRaster(width, height, data, findColorModel(colorModelEnum)) {}

ModernRaster::ModernRaster(const ModernRaster& other) : width(other.width), height(other.height),
                                                        colorModel(other.colorModel) {
    int length = width * height * other.colorModel->getComponentsCount();
    data = std::shared_ptr<float[]>(new float[length]);
    for (int i = 0; i < length; i++) {
        data[i] = other.data[i];
    }
    rgbaData = std::shared_ptr<rgba[]>(new rgba[width * height]);
    fillRgbaData();
}

ModernRaster::ModernRaster(const ModernRaster&& other) noexcept: ModernRaster(other.width, other.height, other.data,
                                                                              other.colorModel) {
    rgbaData = other.rgbaData;
}

int ModernRaster::getWidth() const {
    return width;
}

int ModernRaster::getHeight() const {
    return height;
}

const unsigned char* ModernRaster::getRgbaData() const {
    return (unsigned char*) rgbaData.get();
}

PixelType ModernRaster::getPixelType() const {
    throw std::runtime_error("(");
}

ModernRaster* ModernRaster::clone() const {
    return new ModernRaster(*this);
}

const ColorModel* ModernRaster::getColorModel() const {
    return colorModel;
}

const float* ModernRaster::getData() const {
    return data.get();
}

void ModernRaster::reinterpretColorModel(const ColorModelEnum colorModelEnum) {
    if (this->colorModel->getEnum() == colorModelEnum)
        return;
    auto newColorModel = findColorModel(colorModelEnum);
    if (this->colorModel->getComponentsCount() != newColorModel->getComponentsCount())
        throw std::runtime_error("color model components count must be the same");

    this->colorModel = newColorModel;
    fillRgbaData();
}

void ModernRaster::reinterpretGamma(float gamma) {
    this->gamma = gamma;
    fillRgbaData();
}

void ModernRaster::convertToColorModel(const ColorModelEnum colorModelEnum) {
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

void ModernRaster::setFilter(int index, bool value) {
    if (filter[index] == value)
        return;
    filter[index] = value;
    fillRgbaData();
}

bool ModernRaster::getFilter(int index) const {
    return filter[index];
}

void ModernRaster::fillRgbaData() {
    int length = width * height;
    const float* dataPtr = data.get();
    int componentsCount = colorModel->getComponentsCount();
    float components[4];
    for (int i = 0; i < length; i++) {
        for (int c = 0; c < componentsCount; c++)
            components[c] = (filter[c] ? std::pow(dataPtr[c], 1 / gamma) : 0);
        rgbaData[i] = colorModel->toRgba(components).toRgba();
        dataPtr += componentsCount;
    }
}

void ModernRaster::convertToNewGamma(float gamma) {
    int length = width * height;
    float* dataPtr = data.get();
    int componentsCount = colorModel->getComponentsCount();
    for (int i = 0; i < length; i++) {
        auto rgbaf = colorModel->toRgba(dataPtr);

        rgbaf.r = powf(rgbaf.r, gamma / gamma);
        rgbaf.g = powf(rgbaf.g, gamma / gamma);
        rgbaf.b = powf(rgbaf.b, gamma / gamma);

        colorModel->fromRgb(rgbaf.r, rgbaf.g, rgbaf.b, dataPtr);
        dataPtr += componentsCount;
    }

    gamma = gamma;

    fillRgbaData();
}
