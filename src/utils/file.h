//
// Created by danil on 16.09.2022.
//

#ifndef MEDGRAPHICS_FILE_H
#define MEDGRAPHICS_FILE_H

#include <string>
#include <vector>

namespace utils {
    std::string readAllText(const std::string& fileName);

    std::vector<char> readAllBytes(const std::string& fileName);
}

#endif //MEDGRAPHICS_FILE_H
