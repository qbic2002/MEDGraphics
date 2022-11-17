//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RASTER_H
#define MEDGRAPHICS_RASTER_H

#include <iostream>
#include "Pixel.h"
#include "PixelRGBA8.h"
#include "rgba.h"
#include "AbstractRaster.h"

template<class T> requires CheckPixelType<T>
class Raster : public AbstractRaster {
public:
    Raster() = default;

    Raster(int width, int height) : width_(width), height_(height) {
        raster = new T[width_ * height_];
        rgbaRaster = (rgba*) malloc(width_ * height_ * sizeof(rgba));
    }

    Raster(const Raster& other) : Raster(other.width_, other.height_) {
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                this->set(j, i, other.get(j, i));
            }
        }
    }

    int getWidth() const override {
        return width_;
    }

    int getHeight() const override {
        return height_;
    }

    Raster& operator=(const Raster& other) {
        if (&other == this) {
            return *this;
        }

        delete[] raster;
        free(rgbaRaster);

        width_ = other.width_;
        height_ = other.height_;

        raster = new T[width_ * height_];
        rgbaRaster = (rgba*) malloc(width_ * height_ * sizeof(rgba));

        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                this->set(j, i, other.get(j, i));
            }
        }

        return *this;
    }

    ~Raster() override {
        delete[] raster;
        free(rgbaRaster);
    }

    const unsigned char* getRgbaData() const override {
        return (const unsigned char*) rgbaRaster;
    }

    T get(int x, int y) const {
        if (x < 0 || x >= width_) {
            return {};
        }
        if (y < 0 || y >= height_) {
            return {};
        }

        return raster[y * width_ + x];
    }

    T get(unsigned int i) const {
        if (i >= width_ * height_) {
            return {};
        }

        return raster[i];
    }

    void set(int x, int y, const T& pixel) {
        if (x < 0 || x >= width_) {
            return;
        }
        if (y < 0 || y >= height_) {
            return;
        }

        raster[y * width_ + x] = pixel;
        rgbaRaster[y * width_ + x] = pixel.toRGBA();
    }

    void set(unsigned int index, const T& pixel) {
        if (index < 0 || index >= width_ * height_) {
            return;
        }

        raster[index] = pixel;
        rgbaRaster[index] = pixel.toRGBA();
    }

    PixelType getPixelType() const override {
        return utils::getPixelType<T>();
    }

    T* begin() {
        return (raster);
    }

    T* end() {
        return raster + width_ * height_;
    }

    const T* cbegin() const {
        return (raster);
    }

    const T* cend() const {
        return raster + width_ * height_;
    }

    const T* getRaster() const {
        return raster;
    }

    AbstractRaster* clone() const override {
        return new Raster<T>(*this);
    }

private:
    inline void fillRgbaArray() {
        for (int i = 0; i < width_ * height_; ++i) {
            rgbaRaster[i] = raster[i].toRGBA();
        }
    }

    int width_ = 0;
    int height_ = 0;
    T* raster = nullptr;
    rgba* rgbaRaster = nullptr;
};

#endif //MEDGRAPHICS_RASTER_H
