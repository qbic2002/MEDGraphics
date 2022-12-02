//
// Created by golov on 30.11.2022.
//


#include "img/ModernColorModel.h"
#include "utils/measureTime.h"

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

void ModernRaster::convertToColorModel(const ColorModelEnum colorModelEnum) {
    if (colorModel->getEnum() == colorModelEnum)
        return;
    auto destColorModel = findColorModel(colorModelEnum);
    int length = width * height;
    int readComponentsCount = colorModel->getComponentsCount();
    int writeComponentsCount = destColorModel->getComponentsCount();
    float* readPtr = data.get();
    auto* writePtr = new float[length * writeComponentsCount];
    for (int i = 0; i < length; i++) {
        auto rgbaf = colorModel->toRgba(readPtr);
        destColorModel->fromRgba(rgbaf, writePtr);
        readPtr += readComponentsCount;
        writePtr += writeComponentsCount;
    }

    data.reset(writePtr - length * writeComponentsCount);
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
    utils::TimeStamp timeStamp;

    int length = width * height;
    int componentsCount = colorModel->getComponentsCount();
    float components[componentsCount];

    float* dataPtr = ditheringMethodEnum != NO_DITHERING ? dither(data.get()) : data.get();

    auto rgbaFData = new rgbaF[length];
    for (int i = 0; i < length; i++) {
        for (int c = 0; c < componentsCount; c++)
            components[c] = (filter[c] ? dataPtr[i * componentsCount + c] : 0);
        auto rgbaf = colorModel->toRgba(components);
        rgbaFData[i] = rgbaf;
    }

    if (ditheringMethodEnum != NO_DITHERING)
        delete[] dataPtr;

    for (int i = 0; i < length; i++) {
        for (int c = 0; c < 3; c++)
            rgbaFData[i].components[c] = std::pow(rgbaFData[i].components[c], 1 / gamma);
        rgbaData[i] = rgbaFData[i].toRgba();
    }

    delete[] rgbaFData;
    timeStamp.report("fillRgbaData");
}

void ModernRaster::reinterpretGamma(float gamma) {
    this->gamma = gamma;
    fillRgbaData();
}

void ModernRaster::convertToGamma(float gamma_) {
    int length = width * height;
    float* dataPtr = data.get();
    int componentsCount = colorModel->getComponentsCount();
    for (int i = 0; i < length; i++) {
        auto rgbaf = colorModel->toRgba(dataPtr);

        rgbaf.r = std::pow(rgbaf.r, gamma_ / gamma);
        rgbaf.g = std::pow(rgbaf.g, gamma_ / gamma);
        rgbaf.b = std::pow(rgbaf.b, gamma_ / gamma);

        colorModel->fromRgba(rgbaf, dataPtr);
        dataPtr += componentsCount;
    }
    gamma = gamma_;

    fillRgbaData();
}

float ModernRaster::getGamma() const {
    return gamma;
}

/// @return tmpData must be deleted
float* ModernRaster::dither(float* dataPtr) {
    utils::TimeStamp timeStamp;
    auto ditheringMethod = findDitheringMethod(ditheringMethodEnum);

    int length = width * height;
    int componentsCount = colorModel->getComponentsCount();

    auto* tmpData = new float[length * componentsCount];
    for (int c = 0; c < componentsCount; c++) {
        ditheringMethod->dither(ditheringBits, dataPtr + c, tmpData + c, componentsCount, width, height);
    }

    timeStamp.report("dither");
    return tmpData;
}

void ModernRaster::setDitheringMethod(DitheringMethodEnum method) {
    if (ditheringMethodEnum == method)
        return;
    ditheringMethodEnum = method;
    fillRgbaData();
}

DitheringMethodEnum ModernRaster::getDitheringMethodEnum() const {
    return ditheringMethodEnum;
}

void ModernRaster::setDitheringBits(int bits) {
    if (ditheringBits == bits)
        return;
    ditheringBits = bits;
    if (ditheringMethodEnum == NO_DITHERING)
        return;
    fillRgbaData();
}

int ModernRaster::getDitheringBits() const {
    return ditheringBits;
}

inline float distance(Point p1, Point p2, Point t) {
    auto lineVector = p2 - p1;
    float dist = lineVector.dot(t - p1);
    if (dist < 0)
        return t.dist(p1);
    dist = -lineVector.dot(t - p2);
    if (dist < 0)
        return t.dist(p2);

    float result = std::abs((p2.y - p1.y) * t.x - (p2.x - p1.x) * t.y + p2.x * p1.y - p2.y * p1.x) /
                   std::sqrt(std::pow(p2.y - p1.y, 2) + std::pow(p2.x - p1.x, 2));

    return result;
}

void ModernRaster::drawLine(Point p1, Point p2, float* color_, float lineWidth, float opacity) {
    rgbaF color = colorModel->toRgba(color_);
    color.a = opacity;

    int maxX = std::max(p1.x, p2.x);
    maxX += lineWidth;
    maxX = std::min(maxX, width - 1);

    int minX = std::min(p1.x, p2.x);
    minX -= lineWidth;
    minX = std::max(minX, 0);

    int maxY = std::max(p1.y, p2.y);
    maxY += lineWidth;
    maxY = std::min(maxY, height - 1);

    int minY = std::min(p1.y, p2.y);
    minY -= lineWidth;
    minY = std::max(minY, 0);

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            float dist = distance(p1, p2, {x, y});
            if (dist < lineWidth / 2) {
                rgbaF newColor{.a = 1};
                for (int i = 0; i < 3; ++i) {
                    newColor.components[i] =
                            color.components[i] * color.a + (getPixel(x, y).components[i] * (1 - color.a));
//                    newColor.components[i] = (color.components[i] * topOpacity + oldColor.components[i]) / 2;
                    if (newColor.components[i] > 1) newColor.components[i] = 1;
                    if (newColor.components[i] < 0) newColor.components[i] = 0;
                }
                setPixel(newColor, x, y);
            } else {
                float topOpacity = 1 - (dist - lineWidth / 2);
                if (topOpacity > 1 || topOpacity < 0) {
                    continue;
                }

                topOpacity *= color.a;
                rgbaF oldColor = getPixel(x, y);
                rgbaF newColor{.a = 1};
                for (int i = 0; i < 3; ++i) {
                    newColor.components[i] =
                            color.components[i] * topOpacity + (oldColor.components[i] * (1 - topOpacity));
//                    newColor.components[i] = (color.components[i] * topOpacity + oldColor.components[i]) / 2;
                    if (newColor.components[i] > 1) newColor.components[i] = 1;
                    if (newColor.components[i] < 0) newColor.components[i] = 0;
                }

                setPixel(newColor, x, y);
            }
        }
    }
}

void ModernRaster::setPixel(rgbaF rgbaf, int x, int y) {
//    for (int i = 0; i < 3; ++i){
//        rgbaf.components[i] = std::pow(rgbaf.components[i], gamma);
//    }
    int index = (y * width + x);
    colorModel->fromRgba(rgbaf, data.get() + index * colorModel->getComponentsCount());

    float components[4] = {0, 0, 0, 0};
    for (int c = 0; c < colorModel->getComponentsCount(); c++)
        components[c] = (filter[c] ? data.get()[index * colorModel->getComponentsCount() + c] : 0);
    rgbaf = colorModel->toRgba(components);
    for (int i = 0; i < 3; ++i) {
        rgbaf.components[i] = std::pow(rgbaf.components[i], 1 / gamma);
    }
    rgbaData[index] = rgbaf.toRgba();
}

rgbaF ModernRaster::getPixel(int x, int y) {
    rgbaF rgbaf = colorModel->toRgba(data.get() + (y * width + x) * colorModel->getComponentsCount());
    return rgbaf;
}
