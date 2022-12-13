//
// Created by golov on 08.12.2022.
//

#ifndef MEDGRAPHICS_PNGUTILS_H
#define MEDGRAPHICS_PNGUTILS_H

#include <filesystem>
#include "PNGImage.h"

namespace png {
    ModernRaster readPNGImageFromMemory(const char* data, unsigned int length);

    void writePNGImage(const ModernRaster& modernRaster, const std::filesystem::path& filename);
}

#endif //MEDGRAPHICS_PNGUTILS_H
