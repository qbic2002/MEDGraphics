//
// Created by danil on 18.09.2022.
//

#ifndef MEDGRAPHICS_IMAGELOADER_H
#define MEDGRAPHICS_IMAGELOADER_H

#include <string>
#include <vector>
#include <filesystem>
#include "AbstractRaster.h"
#include "ModernColorModel.h"

namespace img {
    ModernRaster* loadImageData(const std::filesystem::path& file);

    ModernRaster* loadImageData(const std::vector<char>& bytes);

    bool isImage(const std::filesystem::path& file) noexcept;
}

#endif //MEDGRAPHICS_IMAGELOADER_H
