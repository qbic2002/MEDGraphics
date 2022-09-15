//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RGBADATA_H
#define MEDGRAPHICS_RGBADATA_H


#include "RGBARaster.h"

class RGBAData {
public:
    RGBAData(int width, int height) : width_(width), height_(height) {
        pRgbaRaster_ = new RGBARaster*[width_];
        for (int i = 0; i < width; ++i) {
            pRgbaRaster_[i] = new RGBARaster[height_];
        }
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
private:
    int width_;
    int height_;
    RGBARaster** pRgbaRaster_;
};


#endif //MEDGRAPHICS_RGBADATA_H
