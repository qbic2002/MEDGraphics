//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBADATA_H
#define MEDGRAPHICS_RGBADATA_H


#include "RGBAPixel.h"

class RGBAData {
public:
    RGBAData() = default;

    RGBAData(int width, int height) : width_(width), height_(height) {
        pRgbaRaster_ = new RGBAPixel[width_ * height_];
//        for (int i = 0; i < width_; ++i) {
//            pRgbaRaster_[i] = new RGBAPixel[height_];
//        }
    }

    RGBAData(const RGBAData &other) : RGBAData(other.width_, other.height_) {
        for (int w = 0; w < width_; ++w) {
            for (int h = 0; h < height_; ++h) {
                set(w, h, other.get(w, h));
            }
        }
    }

    RGBAData &operator=(const RGBAData &other) {
        if (&other == this) {
            return *this;
        }

        delete[] pRgbaRaster_;

        width_ = other.width_;
        height_ = other.height_;

        pRgbaRaster_ = new RGBAPixel[width_ * height_];

        memcpy(pRgbaRaster_, other.pRgbaRaster_, width_ * height_);

        return *this;
    }

    ~RGBAData() {
        delete[] pRgbaRaster_;
    }

    const RGBAPixel *getPRgbaRaster() const {
        return pRgbaRaster_;
    }

    RGBAPixel get(int x, int y) const {
        if (x < 0 || x >= width_) {
            return {};
        }
        if (y < 0 || y >= height_) {
            return {};
        }

        return pRgbaRaster_[y * width_ + x];
    }

    void set(int x, int y, const RGBAPixel &raster) {
        if (x < 0 || x >= width_) {
            return;
        }
        if (y < 0 || y >= height_) {
            return;
        }

        pRgbaRaster_[y * width_ + x] = raster;
    }

private:
    int width_;
    int height_;
    RGBAPixel *pRgbaRaster_;
};


#endif //MEDGRAPHICS_RGBADATA_H
