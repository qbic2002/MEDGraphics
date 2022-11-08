//
// Created by golov on 13.10.2022.
//

#ifndef MEDGRAPHICS_EXPLORERUTILS_H
#define MEDGRAPHICS_EXPLORERUTILS_H

#include <string>

namespace utils {

    /// Call explorer to choose file to open
    /// \param filename
    /// \return true if result is got, false otherwise
    bool getOpenFileName(std::wstring& filename);

    /// Call explorer to choose file to save into
    /// \param filename
    /// \return true if result is got, false otherwise
    bool getSaveFileName(std::wstring& filename);
}
#endif //MEDGRAPHICS_EXPLORERUTILS_H
