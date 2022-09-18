//
// Created by danil on 16.09.2022.
//

#include <iostream>
#include <fstream>
#include "file.h"

namespace utils {
    std::ifstream openFileStream(const std::string& fileName, const std::ios_base::openmode& mode) {
        std::ifstream ifs(fileName, mode);
        if (!ifs.is_open()) {
            std::cerr << "Could not open file '" << fileName << "'" << std::endl;
            throw std::exception();
        }
        return ifs;
    }

    std::string readAllText(const std::string& fileName) {
        std::ifstream ifs = openFileStream(fileName, std::ios::in);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        return content;
    }

    std::vector<char> readAllBytes(const std::string& fileName) {
        std::ifstream ifs = openFileStream(fileName, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type length = ifs.tellg();

        std::vector<char> content(length);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&content[0], length);

        return content;
    }
}