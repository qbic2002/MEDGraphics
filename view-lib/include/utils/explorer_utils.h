//
// Created by golov on 13.10.2022.
//

#ifndef MEDGRAPHICS_EXPLORER_UTILS_H
#define MEDGRAPHICS_EXPLORER_UTILS_H

#include <string>
#include <vector>
#include "utils/ImageFormats.h"

namespace utils {

    struct SaveFormats {
        ImageFormats format;
        std::wstring extension;

        std::vector<std::pair<std::wstring, std::wstring>> subformats;
    };

    static const std::vector<SaveFormats> saveFormats{
            {
                    .format = ImageFormats::PNM,
                    .extension = L".pnm",
                    .subformats {
                            {L"Color image", L".ppm"},
                            {L"Gray image",  L".pgm"}
                    }
            }
    };

    static std::wstring filterString;

    struct OpenFileInfo {
        bool isCancelled;
        std::wstring filename;
        int filterIndex;
    };

    /// Call explorer to choose file to open
    /// @param filename
    /// @return true if result is got, false otherwise
    bool getOpenFileName(std::wstring& filename);

    /// Call explorer to choose file to save into
    /// @param filename
    /// @return true if result is got, false otherwise
    OpenFileInfo getSaveFileName();

    std::wstring fixFileName(const std::wstring& filename, int filterIndex);
}
#endif //MEDGRAPHICS_EXPLORER_UTILS_H
