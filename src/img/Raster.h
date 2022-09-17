//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RASTER_H
#define MEDGRAPHICS_RASTER_H

#include <iostream>
#include "Pixel.h"
#include "RGBAPixel.h"

template<class T>
concept CheckType = std::is_base_of<Pixel, T>::value;


template<class T> requires CheckType<T>
class Raster {
public:
    Raster() = default;

    Raster(int width, int height) : width_(width), height_(height) {
        raster = new T[width_ * height_];
        rgbaRaster = new unsigned char[width_ * height_ * 4];
    }

    Raster(const Raster& other) : Raster(other.width_, other.height_) {
        memcpy(raster, other.raster, width_ * height_ * sizeof(T));
        memcpy(rgbaRaster, other.rgbaRaster, width_ * height_ * 4);
    }

    Raster& operator=(const Raster& other) {
        if (&other == this) {
            return *this;
        }

        delete[] raster;
        delete[] rgbaRaster;

        width_ = other.width_;
        height_ = other.height_;

        raster = new T[width_ * height_];
        memcpy(raster, other.raster, width_ * height_ * sizeof(T));

        rgbaRaster = new unsigned char[width_ * height_ * 4];
        memcpy(rgbaRaster, other.rgbaRaster, width_ * height_ * 4);

        return *this;
    }

    ~Raster() {
        delete[] raster;
        delete[] rgbaRaster;
    }


    const unsigned char* getRgbaRaster() const {
        return rgbaRaster;
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

    void set(int x, int y, const T& pixel) {
        if (x < 0 || x >= width_) {
            return;
        }
        if (y < 0 || y >= height_) {
            return;
        }

        raster[y * width_ + x] = pixel;

        RGBAPixel rgbaPixel = pixel.getRGBA();
        rgbaRaster[(y * width_ + x) * 4] = rgbaPixel.R;
        rgbaRaster[(y * width_ + x) * 4 + 1] = rgbaPixel.G;
        rgbaRaster[(y * width_ + x) * 4 + 2] = rgbaPixel.B;
        rgbaRaster[(y * width_ + x) * 4 + 3] = rgbaPixel.A;
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

private:
    inline void fillRgbaArray() {
        int k = 0;
        for (int i = 0; i < width_ * height_; ++i) {
            rgbaRaster[k++] = raster[i].getRGBA().R;
            rgbaRaster[k++] = raster[i].getRGBA().G;
            rgbaRaster[k++] = raster[i].getRGBA().B;
            rgbaRaster[k++] = raster[i].getRGBA().A;
        }
    }

    int width_;
    int height_;
    T* raster = nullptr;
    unsigned char* rgbaRaster = nullptr;
};


#endif //MEDGRAPHICS_RASTER_H
