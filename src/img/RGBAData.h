//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBADATA_H
#define MEDGRAPHICS_RGBADATA_H


#include "RGBARaster.h"

class RGBAData {
public:
    RGBAData() = default;
    RGBAData(int width, int height) : width_(width), height_(height) {
        pRgbaRaster_ = new RGBARaster*[width_];
        for (int i = 0; i < width_; ++i) {
            pRgbaRaster_[i] = new RGBARaster[height_];
        }
    }

    RGBAData(const RGBAData& other) : RGBAData(other.width_, other.height_) {
        for (int w = 0; w < width_; ++w) {
            for (int h = 0; h < height_; ++h) {
                set(w, h, other.get(w, h));
            }
        }
    }

    RGBAData& operator=(const RGBAData& other) {
        if (&other == this){
            return *this;
        }

        for (int i = 0; i < width_; ++i) {
            delete[] pRgbaRaster_[i];
        }
        delete[] pRgbaRaster_;

        width_ = other.width_;
        height_ = other.height_;

        pRgbaRaster_ = new RGBARaster*[width_];
        for (int i = 0; i < width_; ++i) {
            pRgbaRaster_[i] = new RGBARaster[height_];
        }

        for (int w = 0; w < width_; ++w) {
            for (int h = 0; h < height_; ++h) {
                set(w, h, other.get(w, h));
            }
        }

        return *this;
    }

    ~RGBAData(){
        for (int i = 0; i < width_; ++i) {
            delete[] pRgbaRaster_[i];
        }
        delete[] pRgbaRaster_;
    }

    RGBARaster **getPRgbaRaster() const {
        return pRgbaRaster_;
    }

    RGBARaster get(int w, int h) const {
        if (w < 0 || w >= width_){
            return {};
        }
        if (h < 0 || h >= height_){
            return {};
        }

        return pRgbaRaster_[w][h];
    }

    void set(int w, int h, RGBARaster raster) {
        if (w < 0 || w >= width_){
            return;
        }
        if (h < 0 || h >= height_){
            return;
        }

        pRgbaRaster_[w][h] = raster;
    }
private:
    int width_;
    int height_;
    RGBARaster** pRgbaRaster_;
};


#endif //MEDGRAPHICS_RGBADATA_H
