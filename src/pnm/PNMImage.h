//
// Created by golov on 15.09.2022.
//

#ifndef CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
#define CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H

#include <vector>
#include "../img/Raster.h"

enum PNMMode {
    P5,
    P6
};

class PNMHeader {
public:
    PNMMode pnmMode;
    int width;
    int height;
    int maxGray;
};

class PNMMeta {
public:
    void addMeta(const std::string& meta, int offset);

    const std::vector<std::pair<std::string, int>>& getMeta() const;

private:
    std::vector<std::pair<std::string, int>> meta_{};
};

class PNMImage {
public:

    PNMImage() = default;

    PNMImage(const AbstractRaster* abstractRaster);

    PNMImage(const PNMImage& other);

    PNMImage(PNMImage&& other) noexcept;

    ~PNMImage();

    PNMImage& operator=(const PNMImage& other) = delete;

    PNMHeader pnmHeader;
    PNMMeta pnmMeta;
    AbstractRaster* data = nullptr;
};


#endif //CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
