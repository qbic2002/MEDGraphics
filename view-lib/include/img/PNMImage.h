//
// Created by golov on 15.09.2022.
//

#ifndef CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
#define CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H

#include <vector>
#include "../img/Raster.h"
#include "ModernColorModel.h"

enum PNMMode {
    P5,
    P6
};

class PNMHeader {
public:
    PNMMode mode;
    int width;
    int height;
    int maxValue;
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
    PNMImage(const PNMHeader& header, const PNMMeta& meta, const ModernRaster& raster);

    explicit PNMImage(const ModernRaster& raster);

    PNMImage(const PNMImage& other);

    PNMImage& operator=(const PNMImage& other) = delete;

    PNMHeader header;
    PNMMeta meta;
    ModernRaster raster;
};


#endif //CG22_PROJECT_MEDGRAPHICS_PNMIMAGE_H
