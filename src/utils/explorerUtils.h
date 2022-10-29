//
// Created by golov on 13.10.2022.
//

#ifndef MEDGRAPHICS_EXPLORERUTILS_H
#define MEDGRAPHICS_EXPLORERUTILS_H

#include <string>

namespace utils {

    /// Call explorer to choose file to open
    /// \param fileName
    /// \return true if result is got, false otherwise
    bool getOpenFileName(std::wstring& fileName);

    /// Call explorer to choose file to save into
    /// \param fileName
    /// \return true if result is got, false otherwise
    bool getSaveFileName(std::wstring& fileName);
}
#endif //MEDGRAPHICS_EXPLORERUTILS_H
