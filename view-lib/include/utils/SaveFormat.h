//
// Created by danil on 14.12.2022.
//

#ifndef MEDGRAPHICS_SAVE_FORMAT_H
#define MEDGRAPHICS_SAVE_FORMAT_H

#include <string>
#include <vector>
#include <filesystem>
#include "img/ModernColorModel.h"

namespace utils {

    struct SaveFormat {
        std::wstring label;
        std::vector<std::wstring> possibleExtensions;
        std::wstring defaultExtension;
        std::function<void(const ModernRaster*, const std::filesystem::path&)> saveFunction;

        std::wstring toFilterString() const;

        void saveImage(const ModernRaster* raster, std::wstring& enteredFileName) const;

        void fixFileName(std::wstring& fileName) const;
    };

    const wchar_t* getSaveFilterString();

    const SaveFormat& getSaveFormat(int index);

} // utils

#endif //MEDGRAPHICS_SAVE_FORMAT_H
