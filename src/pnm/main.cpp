//
// Created by golov on 15.09.2022.
//

#include <iostream>
#include <fstream>

#include "utils/fileUtil.h"
#include "PNMImage.h"
#include "utils/pnmUtil.h"

int main(){
    std::ifstream inputPnm(R"(D:\CPP\cg22-project-MEDGraphics\src\pnm\rays2.pnm)", std::ios::binary);

    int size = getFileSize(inputPnm);
    char* buffer = new char[size];
    getFileContent(buffer, inputPnm, size);
//    for (int i = 0; i < size; ++i) {
//        if (buffer[i] == ' ')
//            std::cout << "i " << i << ": " << "wh" << "\n";
//        else
//            std::cout << "i " << i << ": " << buffer[i] << "\n";
//    }


    PNMImage pnmImage;

    pnm::parsePnmHeader(buffer, pnmImage);

    std::cout << pnmImage.pnmMode << " " << pnmImage.width << " " << pnmImage.height << " " << pnmImage.maxGrey << "\n";

    delete[] buffer;
    inputPnm.close();
}