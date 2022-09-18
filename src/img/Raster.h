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
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                this->set(j, i, other.get(j, i));
            }
        }
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
        rgbaRaster = new unsigned char[width_ * height_ * 4];
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                this->set(j, i, other.get(j, i));
            }
        }


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

    Raster<RGBAPixel> compress(int newWidth, int newHeight) const {
        if (newWidth > width_ || newHeight > height_) {
            throw -1;
        }
        Raster<RGBAPixel> rgbaR(width_, height_);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                rgbaR.set(j, i, this->get(j, i).getRGBA());
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
                        newR += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).R;
                        newG += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).G;
                        newB += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).B;
                        newA += rgbaR.get(j * pixelsInOneInWidth + l, i * pixelsInOneInHeight + k).A;
                    }
                }

                if (i == newHeight - 1 && j == newWidth - 1) {
                    for (int k = 0; k < height_ % newHeight; ++k) {
                        for (int l = 0; l < width_ % newWidth; ++l) {
                            newR += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).R;
                            newG += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).G;
                            newB += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).B;
                            newA += rgbaR.get((width_ - 1 - l), (height_ - 1 - k)).A;
                        }
                    }
                }
                if (i == newHeight - 1) {
                    for (int k = 0; k < height_ % newHeight; ++k) {
                        for (int l = 0; l < pixelsInOneInWidth; ++l) {
                            newR += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).R;
                            newG += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).G;
                            newB += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).B;
                            newA += rgbaR.get(j * pixelsInOneInWidth + l, (height_ - 1 - k)).A;
                        }
                    }
                }
                if (j == newWidth - 1) {
                    for (int k = 0; k < width_ % newWidth; ++k) {
                        for (int l = 0; l < pixelsInOneInHeight; ++l) {
                            newR += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).R;
                            newG += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).G;
                            newB += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).B;
                            newA += rgbaR.get((width_ - 1 - k), (i * pixelsInOneInHeight + l)).A;
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
