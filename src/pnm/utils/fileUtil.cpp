//
// Created by golov on 15.09.2022.
//

#include <iostream>
#include "fileUtil.h"

bool getFileContent(char *buffer, std::ifstream &fileStream, int size) {
    if (!fileStream.is_open()) {
        return false;
    }

    fileStream.read(buffer, size);

    return true;
}

int getFileSize(std::ifstream &fileStream) {
    if (!fileStream.is_open()) {
        return -1;
    }

    auto pos = fileStream.tellg();
    fileStream.seekg(0, std::ios::end);
    int size = fileStream.tellg();
    fileStream.seekg(pos);

    return size;
}
