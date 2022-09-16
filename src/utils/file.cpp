//
// Created by danil on 16.09.2022.
//

#include <string>
#include <iostream>
#include <fstream>
#include "file.h"

namespace utils {
    std::string readAllText(const std::string& fileName) {
        std::ifstream ifs(fileName);
        if (!ifs.is_open()) {
            std::cerr << "Could not open file '" << fileName << "'" << std::endl;
            throw std::exception();
        }
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        return content;
    }
}