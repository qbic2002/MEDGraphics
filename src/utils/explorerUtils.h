//
// Created by golov on 13.10.2022.
//

#ifndef MEDGRAPHICS_EXPLORERUTILS_H
#define MEDGRAPHICS_EXPLORERUTILS_H

#include <string>
#include <windows.h>
#include <Commdlg.h>
#include <iostream>

namespace utils {
    std::string getOpenFileName();

    std::string getSaveFileName();
}
#endif //MEDGRAPHICS_EXPLORERUTILS_H
