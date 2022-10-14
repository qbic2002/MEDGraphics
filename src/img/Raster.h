//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_RASTER_H
#define MEDGRAPHICS_RASTER_H

#include <iostream>
#include "Pixel.h"
#include "RGBAPixel.h"
#include "rgba.h"
#include "AbstractRaster.h"

template<class T> requires CheckType<T>
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

    Raster<RGBAPixel> compress(int newWidth, int newHeight) const override {
        if (newWidth > width_ || newHeight > height_) {
            throw std::exception();
        }
        Raster<RGBAPixel> rgbaR(width_, height_);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                rgbaR.set(j, i, RGBAPixel(this->get(j, i).toRGBA()));
            }
        }

        Raster<RGBAPixel> rgbaCompressed(newWidth, newHeight);
        int pixelsInOneInWidth = width_ / newWidth;
        int pixelsInOneInHeight = height_ / newHeight;

        for (int i = 0; i < newHeight; ++i) {
            for (int j = 0; j < newWidth; ++j) {
                int newR = 0;
                int newG = 0;
                int newB = 0;
                int newA = 0;

                for (int k = 0; k < pixelsInOneInHeight; ++k) {
                    for (int l = 0; l < pixelsInOneInWidth; ++l) {
                        newR += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).r;
                        newG += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).g;
                        newB += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).b;
                        newA += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).a;
                    }
                }

                if (i == newHeight - 1 && j == newWidth - 1) {
                    for (int k = 0; k < height_ % newHeight; ++k) {
                        for (int l = 0; l < width_ % newWidth; ++l) {
                            newR += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).r;
                            newG += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).g;
                            newB += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).b;
                            newA += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).a;
                        }
                    }
                }
                if (i == newHeight - 1) {
                    for (int k = 0; k < height_ % newHeight; ++k) {
                        for (int l = 0; l < pixelsInOneInWidth; ++l) {
                            newR += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).r;
                            newG += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).g;
                            newB += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).b;
                            newA += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).a;
                        }
                    }
                }
                if (j == newWidth - 1) {
                    for (int k = 0; k < width_ % newWidth; ++k) {
                        for (int l = 0; l < pixelsInOneInHeight; ++l) {
                            newR += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).r;
                            newG += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).g;
                            newB += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).b;
                            newA += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).a;
                        }
                    }
                }

                if (i == newHeight - 1 && j == newWidth - 1) {
                    int n = ((height_ % newHeight) * (width_ % newWidth) + pixelsInOneInWidth * pixelsInOneInHeight +
                             (height_ % newHeight) * pixelsInOneInWidth + (width_ % newWidth) * pixelsInOneInHeight);
                    newR /= n;
                    newG /= n;
                    newB /= n;
                    newA /= n;
                } else if (i == newHeight - 1) {
                    int n = (pixelsInOneInWidth * pixelsInOneInHeight + (height_ % newHeight) * pixelsInOneInWidth);
                    newR /= n;
                    newG /= n;
                    newB /= n;
                    newA /= n;
                } else if (j == newWidth - 1) {
                    int n = (pixelsInOneInWidth * pixelsInOneInHeight + (width_ % newWidth) * pixelsInOneInHeight);
                    newR /= n;
                    newG /= n;
                    newB /= n;
                    newA /= n;
                } else {
                    int n = (pixelsInOneInHeight * pixelsInOneInWidth);
                    newR /= n;
                    newG /= n;
                    newB /= n;
                    newA /= n;
                }

                rgbaCompressed.set(j, i, RGBAPixel(newR, newG, newB, newA));
            }
        }

        return rgbaCompressed;
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

private:
    inline void fillRgbaArray() {
        for (int i = 0; i < width_ * height_; ++i) {
            rgbaRaster[i] = raster[i].toRGBA();
        }
    }

    int width_;
    int height_;
    T* raster = nullptr;
    rgba* rgbaRaster = nullptr;
};

#endif //MEDGRAPHICS_RASTER_H
