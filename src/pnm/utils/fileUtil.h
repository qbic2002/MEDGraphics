//
// Created by golov on 15.09.2022.
//

#ifndef MEDGRAPHICS_FILEUTIL_H
#define MEDGRAPHICS_FILEUTIL_H

#include <fstream>

int getFileSize(std::ifstream& fileStream);
bool getFileContent(char* buffer, std::ifstream& fileStream, int size);
std::string readAllText(const std::string& fileName);

#endif //MEDGRAPHICS_FILEUTIL_H