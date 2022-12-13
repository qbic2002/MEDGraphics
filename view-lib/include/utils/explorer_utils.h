//
// Created by golov on 13.10.2022.
//

#ifndef MEDGRAPHICS_EXPLORER_UTILS_H
#define MEDGRAPHICS_EXPLORER_UTILS_H

#include <string>

namespace utils {
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
}
#endif //MEDGRAPHICS_EXPLORER_UTILS_H
